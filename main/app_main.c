// CSTDLIB includes.
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// LowNet includes.
#include "app_chat.h"
#include "app_ping.h"
#include "lownet.h"
#include "serial_io.h"
#include "utility.h"

int util_printable(char c);
uint32_t hex_to_dec(const char *hex_digits);
const char *ERROR_OVERRUN = "ERROR // INPUT OVERRUN";
const char *ERROR_UNKNOWN = "ERROR // PROCESSING FAILURE";
const char *ERROR_COMMAND = "Command error";
const char *ERROR_ARGUMENT = "Argument error";

void app_frame_dispatch(const lownet_frame_t *frame) {
    switch (frame->protocol) {
        case LOWNET_PROTOCOL_RESERVE:
            // Invalid protocol, ignore.
            break;
        case LOWNET_PROTOCOL_TIME:
            // Not handled here.  Time protocol is special case
            break;
        case LOWNET_PROTOCOL_CHAT:
            chat_receive(frame);
            break;

        case LOWNET_PROTOCOL_PING:
            ping_receive(frame);
            break;
    }
}

int acceptable(char *message) {
    for (int i = 0; i < strlen(message); i++) {
        if (util_printable(message[i]) == 0) return 0;
    }
    return 1;
}

void app_main(void) {
    char msg_in[MSG_BUFFER_LENGTH];
    char msg_out[MSG_BUFFER_LENGTH];

    // Initialize the serial services.
    init_serial_service();

    // Initialize the LowNet services.
    lownet_init(app_frame_dispatch);

    serial_write_prompt();
    while (true) {
        memset(msg_in, 0, MSG_BUFFER_LENGTH);
        memset(msg_out, 0, MSG_BUFFER_LENGTH);

        if (!serial_read_line(msg_in)) {
            // Quick & dirty input parse.  Presume input length > 0.
            if (msg_in[0] == '/') {
                if (strncmp(msg_in, "/ping", 5) == 0 && strlen(msg_in) >= 10) {
                    char hex[4];
                    memcpy(hex, &msg_in[6], sizeof(char) * 4);
                    uint8_t node = (uint8_t)strtol(hex, NULL, 16);
                    ping(node);

                    snprintf(msg_out, MSG_BUFFER_LENGTH, "Pinging %02X", node);
                    serial_write_line(msg_out);
                } else if (strcmp(msg_in, "/date") == 0) {
                    lownet_time_t curr_time = lownet_get_time();
                    if (!curr_time.parts && !curr_time.seconds) {
                        serial_write_line("Network time is not available");
                    } else {
                        snprintf(msg_out, MSG_BUFFER_LENGTH, "%ld.%d sec since the course started.", curr_time.seconds, curr_time.parts);
                        serial_write_line(msg_out);
                    }
                } else {
                    serial_write_line(ERROR_COMMAND);
                }
            } else if (msg_in[0] == '@') {
                char hex[4];
                memcpy(hex, &msg_in[1], 4 * sizeof(char));
                uint8_t node = (uint8_t)strtol(hex, NULL, 16);
                if (acceptable(&msg_in[6])) {
                    chat_tell(&msg_in[6], node);
                } else {
                    serial_write_line(ERROR_ARGUMENT);
                }

                printf(&msg_in[6]);
                snprintf(msg_out, MSG_BUFFER_LENGTH, " sent to %02X", node);
                serial_write_line(msg_out);
            } else {
                if (acceptable(msg_in)) {
                    chat_shout(msg_in);
                    printf("Broadcasted message: ");
                    serial_write_line(msg_in);
                } else {
                    serial_write_line(ERROR_ARGUMENT);
                }
            }
        }
    }
}

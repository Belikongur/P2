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
                    char node[5];
                    memcpy(node, &msg_in[6], sizeof(char) * 4);
                    node[4] = '\0';

                    serial_write_line(node);
                    ping((uint8_t)strtol(node, NULL, 16));

                } else if (strcmp(msg_in, "/date") == 0) {
                    lownet_time_t curr_time = lownet_get_time();
                    if (!curr_time.parts && !curr_time.seconds) {
                        serial_write_line("Network time is not available\n");
                    } else {
                        int size = snprintf(NULL, 0, "%ld.%d sec since the course started.", curr_time.seconds, curr_time.parts);
                        char date[size];
                        snprintf(date, size, "%ld.%d sec since the course started.", curr_time.seconds, curr_time.parts);
                        serial_write_line(date);
                    }
                } else {
                    serial_write_line(ERROR_COMMAND);
                }
            } else if (msg_in[0] == '@' && strlen(msg_in) >= 7) {
                if (strcmp(msg_in, "@0xFF") == 0) {
                    size_t len = strlen(msg_in) - 6;
                    char message[len + 1];
                    memcpy(message, &msg_in[6], len);
                    message[len] = '\0';
                    chat_shout(message);
                } else {
                    char dest[5];
                    memcpy(dest, &msg_in[1], 4 * sizeof(char));
                    dest[4] = '\0';
                    uint8_t node = (uint8_t)strtol(dest, NULL, 16);

                    size_t len = strlen(msg_in) - 6;
                    char message[len + 1];
                    memcpy(message, &msg_in[6], len);
                    message[len] = '\0';
                    chat_tell(message, node);
                }
            } else {
                // Default, chat broadcast message.
                //	...?
                chat_shout(msg_in);
            }
        }
    }
}

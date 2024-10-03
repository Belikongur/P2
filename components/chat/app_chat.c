#include "app_chat.h"

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <string.h>

#include "lownet.h"
#include "serial_io.h"
#include "utility.h"
int util_printable(char c);

void chat_receive(const lownet_frame_t *frame) {
    if (frame->destination == lownet_get_device_id()) {
        // This is a tell message, just for us!
        //  ...?
    } else {
        // This is a broadcast shout message.
        // ...?
    }
}

void chat_shout(const char *message) {
    for (int i = 0; i < strlen(message); i++) {
        if (util_printable(message[i]) == 0) {
            int size = snprintf(NULL, 0, "Invalid character: %c", message[i]);
            char unprintable[size + 1];
            snprintf(unprintable, size + 1, "Invalid character: %c", message[i]);
            unprintable[size + 1] = '\0';
            serial_write_line(unprintable);
            return;
        }
    }
    lownet_frame_t frame;
    frame.protocol = LOWNET_PROTOCOL_CHAT;
    frame.destination = 0xFF;
    frame.length = strlen(message);
    memcpy(frame.payload, message, frame.length);
    lownet_send(&frame);
}

void chat_tell(const char *message, uint8_t destination) {
    for (int i = 0; i < strlen(message); i++) {
        if (util_printable(message[i]) == 0) {
            int size = snprintf(NULL, 0, "Invalid character: %c", message[i]);
            char unprintable[size + 1];
            snprintf(unprintable, size + 1, "Invalid character: %c", message[i]);
            unprintable[size + 1] = '\0';
            serial_write_line(unprintable);
            return;
        }
    }
    lownet_frame_t frame;
    frame.protocol = LOWNET_PROTOCOL_CHAT;
    frame.destination = destination;
    frame.length = strlen(message);
    memcpy(frame.payload, message, frame.length);
    lownet_send(&frame);
}

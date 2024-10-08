#include "app_chat.h"

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <string.h>

#include "lownet.h"
#include "serial_io.h"

void chat_receive(const lownet_frame_t *frame) {
    if (frame->destination == lownet_get_device_id()) {
        uint8_t msg_len = frame->length;
        char message[msg_len];
        memcpy(message, frame->payload, msg_len);
        serial_write_line(message);
    } else {
        uint8_t msg_len = frame->length;
        char message[msg_len];
        memcpy(message, frame->payload, msg_len);
        serial_write_line(message);
    }
}

void chat_shout(const char *message) {
    lownet_frame_t frame;
    frame.protocol = LOWNET_PROTOCOL_CHAT;
    frame.destination = 0xFF;
    frame.length = strlen(message);
    memcpy(frame.payload, message, frame.length);
    lownet_send(&frame);
}

void chat_tell(const char *message, uint8_t destination) {
    lownet_frame_t frame;
    frame.protocol = LOWNET_PROTOCOL_CHAT;
    frame.destination = destination;
    frame.length = strlen(message);
    memcpy(frame.payload, message, frame.length);
    lownet_send(&frame);
}

#include "app_ping.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "serial_io.h"

typedef struct __attribute__((__packed__)) {
    lownet_time_t timestamp_out;
    lownet_time_t timestamp_back;
    uint8_t origin;
} ping_packet_t;

void ping(uint8_t node) {
    ping_packet_t ping_packet;
    ping_packet.timestamp_out = lownet_get_time();
    ping_packet.timestamp_back = lownet_get_time();
    ping_packet.origin = lownet_get_device_id();

    lownet_frame_t frame;
    frame.protocol = LOWNET_PROTOCOL_PING;
    frame.destination = node;
    frame.length = sizeof(ping_packet);
    memcpy(frame.payload, &ping_packet, sizeof(ping_packet));
    lownet_send(&frame);
}

void ping_receive(const lownet_frame_t* frame) {
    if (frame->length < sizeof(ping_packet_t)) {
        // Malformed frame.  Discard.
        return;
    }

    // Extract ping packet
    lownet_time_t timestamp_origin, timestamp_response;
    memcpy(&timestamp_origin, frame->payload, sizeof(lownet_time_t));
    memcpy(&timestamp_response, &frame->payload[sizeof(lownet_time_t)], sizeof(lownet_time_t));
    uint8_t origin = frame->payload[sizeof(ping_packet_t) - 1];

    // Log time
    char msg_out[LOWNET_PAYLOAD_SIZE];
    snprintf(msg_out, LOWNET_PAYLOAD_SIZE, "Origin %02X time: %ld.%d, Response %02X time: %ld.%d", origin, timestamp_origin.seconds, timestamp_origin.parts, frame->source, timestamp_response.seconds, timestamp_response.parts);
    serial_write_line(msg_out);

    // uint8_t msg_len = frame->length - sizeof(ping_packet_t);
    uint8_t source = lownet_get_device_id();
    if (origin == source) {
        // 1.case: Receiving a pong as response to sent ping
        // if (!msg_len)
        //    snprintf(msg_out, MSG_BUFFER_LENGTH, "Ping response from %02X was empty", frame->source);
        // else
        snprintf(msg_out, LOWNET_PAYLOAD_SIZE, "%s", &frame->payload[sizeof(ping_packet_t)]);
        serial_write_line(msg_out);
    } else {
        // 2.case: Receiving a ping and respond with pong
        char pong[] = "pong";

        // Check if enough space for pong response
        // if (frame->length + sizeof(pong) > LOWNET_PAYLOAD_SIZE) {
        //    serial_write_line("Not enough space in payload for response \"pong\"");
        //    return;
        //}

        // if (msg_len > 0) {
        //     snprintf(msg_out, MSG_BUFFER_LENGTH, "Ping received from %02X: %.*s", frame->source, msg_len, &frame->payload[sizeof(ping_packet_t)]);
        //     serial_write_line(msg_out);
        // }
        timestamp_response = lownet_get_time();
        uint8_t ping_packet[sizeof(ping_packet_t)];
        memcpy(&ping_packet[sizeof(lownet_time_t)], &timestamp_response, sizeof(lownet_time_t));
        lownet_frame_t frame_response = *frame;
        frame_response.destination = origin;
        frame_response.length = frame->length + sizeof(pong);
        memcpy(frame_response.payload, ping_packet, sizeof(ping_packet_t));
        if (frame->length > sizeof(ping_packet_t)) memcpy(&frame_response.payload[sizeof(ping_packet_t)], &frame->payload[sizeof(ping_packet_t)], frame->length - sizeof(ping_packet_t));
        memcpy(&frame_response.payload[frame->length], pong, sizeof(pong));
        lownet_send(&frame_response);
    }
}

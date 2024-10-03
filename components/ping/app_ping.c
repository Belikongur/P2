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
    ping_packet.timestamp_out = lownet_get_time();
    ping_packet.origin = lownet_get_device_id();

    uint8_t ping[sizeof(ping_packet)];
    lownet_frame_t frame;
    memcpy(ping, &ping_packet, sizeof(ping_packet));
    frame.source = lownet_get_device_id();
    frame.destination = node;
    frame.protocol = LOWNET_PROTOCOL_PING;
    frame.length = sizeof(ping);
    mempcpy(frame.payload, ping, sizeof(ping));
    lownet_send(&frame);
}

void ping_receive(const lownet_frame_t *frame) {
    if (frame->length < sizeof(ping_packet_t)) {
        // Malformed frame.  Discard.
        return;
    }

    // ...?
}

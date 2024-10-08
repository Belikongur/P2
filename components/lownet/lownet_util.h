#ifndef GUARD_LOWNET_UTIL_H
#define GUARD_LOWNET_UTIL_H

#include <lownet.h>
#include <stdint.h>

typedef struct {
    uint8_t mac[6];
    uint8_t node;
} lownet_identifier_t;

lownet_identifier_t lownet_lookup(uint8_t id);
lownet_identifier_t lownet_lookup_mac(const uint8_t* mac);

// void dump_frame(const lownet_frame_t* frame);
// void frame_chat(const lownet_frame_t* frame);
// void frame_ping(const lownet_frame_t* frame);

uint32_t lownet_crc(const lownet_frame_t* frame);

#endif

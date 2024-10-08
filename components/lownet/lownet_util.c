#include "lownet_util.h"

#include <stddef.h>

static const lownet_identifier_t device_table[] =
    {
        // Reserved -- dead identifier.
        {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0},

        {{0x24, 0x0a, 0xc4, 0x60, 0xa4, 0xd0}, 0x10},
        {{0x48, 0xe7, 0x29, 0x99, 0x35, 0x38}, 0x11},
        {{0x24, 0x0a, 0xc4, 0x60, 0x99, 0xe0}, 0x12},
        {{0x24, 0x62, 0xab, 0xf9, 0x01, 0x10}, 0x13},
        {{0x24, 0x62, 0xab, 0xf9, 0x72, 0x64}, 0x14},
        {{0x24, 0x62, 0xab, 0xf8, 0xfd, 0xe4}, 0x15},
        {{0x24, 0x6f, 0x28, 0x7c, 0xeb, 0x58}, 0x16},
        {{0x48, 0xe7, 0x29, 0x99, 0x4b, 0x84}, 0x17},
        {{0x48, 0xe7, 0x29, 0x99, 0x2c, 0x0c}, 0x18},
        {{0x48, 0xe7, 0x29, 0x99, 0x53, 0x7c}, 0x19},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x1f, 0x8c}, 0x1a},
        {{0x24, 0x62, 0xab, 0xf9, 0x5f, 0xf8}, 0x1b},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x1e, 0xdc}, 0x1c},
        {{0x24, 0x6f, 0x28, 0x7b, 0x0f, 0x38}, 0x1d},
        {{0x48, 0xe7, 0x29, 0x98, 0xe9, 0x30}, 0x1e},
        {{0x48, 0xe7, 0x29, 0x99, 0x2a, 0x90}, 0x1f},
        {{0x48, 0xe7, 0x29, 0x99, 0x34, 0xb4}, 0x20},
        {{0x48, 0xe7, 0x29, 0x97, 0xe8, 0x14}, 0x21},
        {{0x48, 0xe7, 0x29, 0x99, 0x27, 0x04}, 0x22},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x2c, 0x50}, 0x23},
        {{0x24, 0x0a, 0xc4, 0x61, 0x17, 0x50}, 0x24},
        {{0x24, 0x62, 0xab, 0xe1, 0x12, 0x38}, 0x25},
        {{0x24, 0x6f, 0x28, 0x7b, 0x26, 0x30}, 0x26},
        {{0x24, 0x0a, 0xc4, 0x61, 0x01, 0xe8}, 0x27},
        {{0x24, 0x0a, 0xc4, 0x60, 0x98, 0xb4}, 0x28},
        {{0x24, 0x0a, 0xc4, 0x61, 0x04, 0x20}, 0x29},
        {{0x24, 0x62, 0xab, 0xf9, 0x01, 0xe8}, 0x2a},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x35, 0x3c}, 0x2b},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x51, 0x08}, 0x2c},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x4c, 0xbc}, 0x2d},
        {{0x24, 0x62, 0xab, 0xf9, 0x12, 0x28}, 0x2e},
        {{0x24, 0x6f, 0x28, 0x7b, 0x37, 0x44}, 0x2f},
        {{0x24, 0x0a, 0xc4, 0x61, 0x1f, 0xfc}, 0x30},
        {{0x24, 0x62, 0xab, 0xf8, 0xe7, 0x44}, 0x31},
        {{0x24, 0x62, 0xab, 0xe1, 0x26, 0x7c}, 0x32},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x37, 0x94}, 0x33},
        {{0x24, 0x0a, 0xc4, 0x61, 0x19, 0xc8}, 0x34},
        {{0x48, 0xe7, 0x29, 0x99, 0x37, 0xe4}, 0x35},
        {{0x24, 0x0a, 0xc4, 0x60, 0xbb, 0x78}, 0x36},
        {{0x48, 0xe7, 0x29, 0x99, 0x37, 0x60}, 0x37},
        {{0xb0, 0xa7, 0x32, 0xdb, 0x51, 0x30}, 0x38},
        {{0x24, 0x62, 0xab, 0xf9, 0x40, 0xc8}, 0x39},

        {{0x24, 0x0a, 0xc4, 0x60, 0xb0, 0x48}, 0xE1},
        {{0x24, 0x0a, 0xc4, 0x61, 0x04, 0x54}, 0xE2},

        // Deebugging device, Groska.
        {{0x24, 0x0a, 0xc4, 0x61, 0x04, 0x00}, 0xF0},

        // Broadcast identifier.
        {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 0xFF}};

// Lookup identifier by node id.
lownet_identifier_t lownet_lookup(uint8_t id) {
    size_t size = sizeof(device_table) / sizeof(device_table[0]);
    for (int i = 0; i < size; ++i) {
        if (device_table[i].node == id) {
            return device_table[i];
        }
    }
    return device_table[0];
}

// Lookup identifier by mac address.
lownet_identifier_t lownet_lookup_mac(const uint8_t* mac) {
    size_t size = sizeof(device_table) / sizeof(device_table[0]);
    for (int i = 0; i < size; ++i) {
        int match = 1;
        for (int j = 0; j < 6; ++j) {
            if (device_table[i].mac[j] != mac[j]) {
                match = 0;
            }
        }
        if (match) {
            return device_table[i];
        }
    }
    return device_table[0];
}

uint32_t lownet_crc(const lownet_frame_t* frame) {
    uint32_t reg = 0x00777777;                 // Shift register initial vector.
    static const uint32_t poly = 0x1800463ul;  // G(x)

    void process_byte(uint8_t b) {
        for (int i = 0; i < 8; i++) {
            reg = (reg << 1) | (b & 1);
            b = b >> 1;
            if (reg & 0x1000000ul)
                reg = (reg ^ poly);  // take mod G(x)
        }
    }
    const uint8_t* iter = (const uint8_t*)frame;
    int len = LOWNET_FRAME_SIZE - LOWNET_CRC_SIZE;
    for (int i = 0; i < len; i++)
        process_byte(iter[i]);
    return reg;
}

// void dump_frame(const lownet_frame_t* frame) {
//     printf("Frame Dump:\n");
//     printf("Magic: %02X %02X\n", frame->magic[0], frame->magic[1]);
//     printf("Source: %02X\n", frame->source);
//     printf("Destination: %02X\n", frame->destination);
//     printf("Protocol: %02X\n", frame->protocol);
//     printf("Length: %02X\n", frame->length);
//     printf("Padding: %02X %02X\n", frame->padding[0], frame->padding[1]);
//
//     printf("Payload: ");
//     for (size_t i = 0; i < frame->length; i++) {
//         printf("%02X ", frame->payload[i]);
//     }
//     printf("\n");
//
//     printf("CRC: %08lX\n", frame->crc);
//     // frame_chat(frame);
//     // frame_ping(frame);
// }
//
// void frame_chat(const lownet_frame_t* frame) {
//     char message[frame->length];
//     memcpy(message, frame->payload, frame->length);
//     printf("sent from %02X: ", frame->source);
//     serial_write_line(message);
// }
//
// void frame_ping(const lownet_frame_t* frame) {
//     size_t time_len = sizeof(lownet_time_t);
//     size_t PING_PACKET_SIZE = time_len * 2 + 1;
//
//     // Extract ping packet
//     lownet_time_t timestamp_origin, timestamp_response;
//     memcpy(&timestamp_origin, frame->payload, sizeof(lownet_time_t));
//     memcpy(&timestamp_response, &frame->payload[sizeof(lownet_time_t)], sizeof(lownet_time_t));
//     uint8_t origin = frame->payload[PING_PACKET_SIZE - 1];
//
//     // Log time
//     char msg_out[MSG_BUFFER_LENGTH];
//     snprintf(msg_out, MSG_BUFFER_LENGTH, "Origin %02X time: %ld.%d, Response %02X time: %ld.%d", origin, timestamp_origin.seconds, timestamp_origin.parts, frame->source, timestamp_response.seconds, timestamp_response.parts);
//     serial_write_line(msg_out);
//
//     uint8_t msg_len = frame->length - PING_PACKET_SIZE;
//     uint8_t source = lownet_get_device_id();
//     if (origin == source) {
//         // 1.case: Receiving a pong as response to sent ping
//         if (!msg_len)
//             snprintf(msg_out, MSG_BUFFER_LENGTH, "Ping response from %02X was empty", frame->source);
//         else
//             snprintf(msg_out, MSG_BUFFER_LENGTH, "Ping response from %02X: %.*s", frame->source, msg_len, &frame->payload[PING_PACKET_SIZE]);
//         serial_write_line(msg_out);
//     } else {
//         // 2.case: Receiving a ping and respond with pong
//         const char pong[] = "pong";
//
//         // Check if enough space for pong response
//         if (frame->length + sizeof(pong) > LOWNET_PAYLOAD_SIZE) {
//             serial_write_line("Not enough space in payload for response \"pong\"");
//             return;
//         }
//
//         if (msg_len > 0) {
//             snprintf(msg_out, MSG_BUFFER_LENGTH, "Ping received from %02X: %.*s", frame->source, msg_len, &frame->payload[PING_PACKET_SIZE]);
//             serial_write_line(msg_out);
//         }
//         timestamp_response = lownet_get_time();
//         uint8_t payload[LOWNET_PAYLOAD_SIZE];
//         memcpy(&payload[sizeof(lownet_time_t)], &timestamp_response, sizeof(lownet_time_t));
//         lownet_frame_t frame_response = *frame;
//         frame_response.source = source;
//         frame_response.destination = origin;
//         frame_response.length = frame->length + sizeof(pong);
//         memcpy(frame_response.payload, payload, frame->length);
//         memcpy(&frame_response.payload[frame->length], pong, sizeof(pong));
//         lownet_send(&frame_response);
//     }
// }
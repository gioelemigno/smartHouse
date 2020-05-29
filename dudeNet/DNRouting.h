#pragma once

#include <stdint.h>
#include "DNError.h"

#define MAX_PAYLOAD_SIZE    64

typedef uint8_t address_t;

typedef struct packet_t{
    uint8_t start_0;
    uint8_t start_1;

    address_t dst;
    address_t src;

    uint8_t size;
    
    uint8_t payload[MAX_PAYLOAD_SIZE];

    uint8_t crc_1;
    uint8_t crc_0;
}packet_t;

// read packet, if there isn't ready packet wait TIMEOUT_ms
res_t DNRouting_read(packet_t* packet);

// write packet
// NOTE: this function add only start bytes and crc
res_t DNRouting_write(packet_t* packet);
#pragma once

#include <stdint.h>
#include "DNError.h"

#define MAX_PAYLOAD_SIZE    64
#define TIMEOUT_ms          100

#define NO_ADDRESS          0

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


extern address_t myAddress;

//Used by read function
extern packet_t packetRX;

//used by write function
extern packet_t packetTX;

// read packet, if there isn't ready packet wait TIMEOUT_ms
res_t DNRouting_read(packet_t* packet);

// write packet
// NOTE: this function add only start bytes and crc
res_t DNRouting_write(packet_t* packet);

//initialize network
res_t DNRouting_init(unsigned long baud_rate);


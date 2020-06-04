#pragma once

#include <stdint.h>
#include "DNError.h"

#define DN_ROUTING_DEBUG
#define PRINTABLE

//used in crc and tx 
#define PRE_PAYLOAD_LENGTH    2+2+1 //((2B start + dst+src+size))

#define START_0         0xFF
#define START_1         0xFF

// two bits are used to put crc after payload
#define MAX_PAYLOAD_SIZE    64+2
#define TIMEOUT_ms          100

#define NO_ADDRESS          254

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
extern volatile packet_t packetRX;

//used by write function
extern volatile packet_t packetTX;

typedef void(*packetHandler_t)(volatile packet_t*);

//function called when packetRX is ready (ISR)
//used only in Atmega328P
extern volatile packetHandler_t packetHandler;

// read packet, if there isn't ready packet wait TIMEOUT_ms
res_t DNRouting_read(volatile packet_t* packet);

// write packet
// NOTE: this function add only start bytes and crc
res_t DNRouting_write(volatile packet_t* packet);

//initialize network
res_t DNRouting_init(unsigned long baud_rate);

//free resource
res_t DNRouting_close();

#if defined(DN_ROUTING_DEBUG) && defined(PRINTABLE) 
    void DNRouting_printPacket(const char* packet_name, volatile packet_t* packet);
#endif /* DN_ERROR_VERBOSE */
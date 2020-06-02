#include "DNRouting.h"
#include "lib/CRC/CRC.h"
#include "lib/Linux/UART/UART.h"
#include <stdio.h>
address_t myAddress = NO_ADDRESS;

//Used by read function
volatile packet_t packetRX;

//used by write function
packet_t packetTX;

// read packet, if there isn't ready packet wait TIMEOUT_ms
res_t DNRouting_read(packet_t* packet){
    // TODO
    double timeout =  TIMEOUT_ms * 1E03;
    int res = UART_read_waiting(&RXBuffer, &timeout);
    printf("MAIN waited %.2f\n", (TIMEOUT_ms * 1E03 - timeout));

    if(res == -1) printf("nothing to read :(\n");
    
    return -1;
}

// write packet
// NOTE: this function add only start bytes, src (myAddress) and crc
// NOTE: this function add crc after payload to obtain contiguous array
res_t DNRouting_write(packet_t* packet){

    packet->src=myAddress;
    
    crc_t crc = crcFast((unsigned char*) packet, PRE_PAYLOAD_LENGTH + packet->size);
    packet->crc_1 = (uint8_t) (crc>>8);
    packet->crc_0 = (uint8_t) crc;

    packet->payload[packet->size] = packet->crc_1;
    packet->payload[packet->size+1] = packet->crc_0;
        
    TXBuffer.buffer=(uint8_t*)packet;
    TXBuffer.size = PRE_PAYLOAD_LENGTH + packet->size + 2; 
    UART_write(&TXBuffer);
}

//initialize network
res_t DNRouting_init(unsigned long baud_rate){
    UART_init(baud_rate);
    crcInit(); //build lookup table
    
    packetTX.start_0 =START_0;
    packetTX.start_1=START_1;
}

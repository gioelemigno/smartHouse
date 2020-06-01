#include "DNRouting.h"
#include "lib/Atmega328P/UART/UART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib/CRC/CRC.h"

//used in crc and tx 
#define PRE_PAYLOAD_LENGTH    2+2+1 //((2B start + dst+src+size))

typedef struct RX_info_t{
    uint8_t reading_packet :1;
    uint8_t packet_is_addressed_to_me :1;
    uint8_t packet_ready:1;
}RX_info_t;

typedef void(*RX_status_t)(uint8_t);

//wait start_0 
void get_start_0(uint8_t data);

//wait start_1
void get_start_1(uint8_t data);

void get_dst(uint8_t data);
void get_src(uint8_t data);

void get_size(uint8_t data);

void get_payload(uint8_t data);

//get MSB of CRC
void get_CRC(uint8_t data);

//if packet is not addressed to me, ignore payload and crc
// DON'T SAVE PACKET
void ignore_packet(uint8_t data);

volatile RX_status_t RX_status = get_start_0;
//volatile uint8_t payload_size =0;
volatile uint8_t RX_index = 0;

volatile RX_info_t RX_info ={
    .reading_packet=0,
    .packet_is_addressed_to_me=0,
    .packet_ready=0
};

//volatile bool save_packet = false;

address_t myAddress = NO_ADDRESS;

//Used by read function
volatile packet_t packetRX;

//used by write function
packet_t packetTX;

ISR(USART_RX_vect){
    RX_status(UDR0);
}

// read packet, if there isn't ready packet wait TIMEOUT_ms
res_t DNRouting_read(packet_t* packet){

}

// write packet
// NOTE: this function add only start bytes and crc
res_t DNRouting_write(packet_t* packet){

}

//initialize network
res_t DNRouting_init(unsigned long baud_rate){
    UART_init(baud_rate);
    crcInit(); //build lookup table
}



//wait start_0 
void get_start_0(uint8_t data){
    if(data!=START_0)   return;
    packetRX.start_0 = data;

    RX_status = get_start_1;
    RX_info.reading_packet=true;
    RX_info.packet_ready= false;
}

//wait start_1
void get_start_1(uint8_t data){
    if(data!=START_1){
        RX_status = get_start_0;
        return;
    }
    packetRX.start_1 = data;

    RX_status = get_dst;
}

void get_dst(uint8_t data){
    if(data != myAddress){
        RX_info.packet_is_addressed_to_me = false;
    }
    packetRX.dst = data;
    RX_status = get_src;
}

void get_src(uint8_t data){
    packetRX.src = data;

    RX_status = get_size;
    RX_index=0;

}

void get_size(uint8_t data){
    if(data > MAX_PAYLOAD_SIZE-2){
        RX_info.reading_packet=false;
        RX_status = get_start_0;
        return;
    }
    packetRX.size=data;
    if(RX_info.packet_is_addressed_to_me)   RX_status = get_payload;
    else RX_status = ignore_packet;
}


void get_payload(uint8_t data){
    packetRX.payload[RX_index++] = data;
    if(RX_index == packetRX.size){
        RX_status=get_CRC;
    }
}


volatile uint8_t ignore_packet_index = 0;
void ignore_packet(uint8_t data){
    uint8_t trash = data; // try to remove it

    ignore_packet_index++;
    if(ignore_packet_index == packetRX.size+2){
        ignore_packet_index=0;
        RX_status = get_start_0;
    }
}

volatile uint8_t crc_n = 1; // crc_1 or crc_0
//get MSB of CRC
void get_CRC(uint8_t data){
    if(crc_n) {
        packetRX.crc_1=data;
        packetRX.payload[RX_index++] = data;
        crc_n = 0;
        return;
    }
    //end packet
    packetRX.crc_0=data;
    packetRX.payload[RX_index++] = data;
    crc_n = 1;
    
    crc_t crc = crcFast((unsigned char*) &packetRX, PRE_PAYLOAD_LENGTH + packetRX.size + 2);

    if(crc){
        //wrong CRC
        RX_info.reading_packet=false;
        RX_info.packet_ready=false;
    }
    else{
        RX_info.reading_packet=false;
        RX_info.packet_ready=true;
    }
    RX_status=get_start_0;

}
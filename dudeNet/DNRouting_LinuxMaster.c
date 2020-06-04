#include "DNRouting.h"
#include "lib/CRC/CRC.h"
#include "lib/Linux/UART/UART.h"
#include <stdio.h>
#include <stdlib.h>
#include "DNError.h"

#define NO_DATA_TO_READ 0xAAFF // used in getChar() function

#define NOT_ADDRESS_TO_ME   0xF0

address_t myAddress = NO_ADDRESS;

//Used by read function
volatile packet_t packetRX;

//used by write function
volatile packet_t packetTX;

typedef struct RX_info_t{
    uint8_t reading_packet :1;
    uint8_t packet_is_addressed_to_me :1;
    uint8_t packet_ready:1;
}RX_info_t;

RX_info_t RX_info ={
    .reading_packet=0,
    .packet_is_addressed_to_me=0,
    .packet_ready=0
};

//used in get_CRC()
uint8_t crc_n = 1; // crc_1 or crc_0

//used in ignore_packet()
uint16_t ignore_packet_index=0;

uint16_t RX_packet_payload_index = 0;

typedef enum RX_packet_status_t{
    get_start_0 = 0,
    get_start_1,
    get_dst,
    get_src,
    get_size,
    get_payload,
    get_CRC,
    ignore_packet
}RX_packet_status_t;

//indicates current status of packet receiver (build_packet())
RX_packet_status_t RX_packet_status = get_start_0;


//build packet
// return 1 is packet is ready
// return NOT_ADDRESS_TO_ME if there is a ready packet, but it is not for me
static inline uint8_t buildPacket(uint8_t data, volatile packet_t* packet);

//return a bytes from buffer of UART
//can return NO_DATA_TO_READ
static inline uint16_t getChar(double* timeout);

// read packet, if there isn't ready packet wait TIMEOUT_ms
res_t DNRouting_read(volatile packet_t* packet){
    //usleep(1E03); //TODO TEST (reduce syscall)
    double timeout =  TIMEOUT_ms * 1E03;
    uint16_t data = getChar(&timeout);
    if(data==NO_DATA_TO_READ || timeout <= 0.0){
        DNError = ERR_TIMEOUT; 
        DNError_func = __func__;
        return -1;
    }

    while(!buildPacket((uint8_t)data, packet)){
        data = getChar(&timeout);
        if(data==NO_DATA_TO_READ || timeout <= 0.0){
            DNError = ERR_TIMEOUT;
            DNError_func = __func__; 
            return -1;
        }
    }
    
    if(RX_info.packet_is_addressed_to_me)   {
        return packet->size;
    }
    else{
        DNError=ERR_DST;
        DNError_func = __func__;
        return -1;
    }
}

// write packet
// NOTE: this function add only start bytes, src (myAddress) and crc
// NOTE: this function add crc after payload to obtain contiguous array
res_t DNRouting_write(volatile packet_t* packet){

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

//free resource
res_t DNRouting_close(){
    int res = UART_close();
    return res;
}


//return a bytes from buffer of UART
static inline uint16_t getChar(double* timeout){
    if(RXBuffer.index >= RXBuffer.size){
        int res = UART_read_waiting(&RXBuffer, timeout);
        if(res == -1) return NO_DATA_TO_READ;
    } 
    return (uint8_t)( ((uint8_t*)RXBuffer.buffer)[RXBuffer.index++] );
}


static inline uint8_t buildPacket(uint8_t data, volatile packet_t* packet){
    switch(RX_packet_status){
        case get_start_0:
            if(data!=START_0)   return 0;
            packet->start_0 = data;

            RX_info.reading_packet=true;
            RX_info.packet_ready= false;
            RX_info.packet_is_addressed_to_me=false;

            RX_packet_status=get_start_1;
        break;
        
        case get_start_1:
            if(data!=START_1){
                RX_packet_status = get_start_0;
                RX_info.reading_packet=false;
                RX_info.packet_is_addressed_to_me=false;

                return 0;
            }
            packet->start_1 = data;

            RX_packet_status = get_dst;
        break;
        
        case get_dst:
            if(data != myAddress){
                RX_info.packet_is_addressed_to_me = false;
            }
            else RX_info.packet_is_addressed_to_me = true;

            packet->dst = data;
            RX_packet_status = get_src;
        break;
        
        case get_src:
            packet->src = data;
            RX_packet_status = get_size;
        break;
        
        case get_size:
            if(data > MAX_PAYLOAD_SIZE-2){
                RX_info.reading_packet=false;
                RX_packet_status = get_start_0;
                return 0 ;
            }
            
            packet->size=data;
            RX_packet_payload_index=0;
            if(RX_info.packet_is_addressed_to_me)   RX_packet_status = get_payload;
            else RX_packet_status = ignore_packet;
        break;

        case get_payload:
            packet->payload[RX_packet_payload_index++] = data;
            if(RX_packet_payload_index == packet->size){
                RX_packet_status=get_CRC;
                return 0;
            }
        break;
        
        case get_CRC:
            if(crc_n) {
                packet->crc_1=data;
                packet->payload[RX_packet_payload_index++] = data;
                crc_n = 0;
                return 0;
            }
            //end packet
            packet->crc_0=data;
            packet->payload[RX_packet_payload_index++] = data;
            crc_n = 1;
            
            crc_t crc = crcFast((unsigned char*) &packetRX, PRE_PAYLOAD_LENGTH + packet->size + 2);

            if(crc){
                //wrong CRC
                RX_info.reading_packet=false;
                RX_info.packet_ready=false;
            }
            else{
                RX_info.reading_packet=false;
                RX_info.packet_ready=true;
            }

            RX_packet_status=get_start_0;
            RX_info.reading_packet=false;
            
            return 1;//if(RX_info.packet_ready) readyPacketInterrupt_throwINT();
            //RX_info.packet_ready = false;
        break;
        
        case ignore_packet:
            ignore_packet_index++;
            if(ignore_packet_index == packet->size+2){
                ignore_packet_index=0;
                RX_packet_status = get_start_0;

                return NOT_ADDRESS_TO_ME;
            }
        break;

        default:
            fprintf(stderr, "Internal Error\n");
            exit(EXIT_FAILURE);
        break;
    }

    return 0;
}

#if defined(DN_ROUTING_DEBUG) && defined(PRINTABLE) 
    void DNRouting_printPacket(const char* packet_name, volatile packet_t* packet){
        printf("\n%s:\n", packet_name);
        printf("\t| \033[1;33mSTART:\033[0m %2X %2X |", packet->start_0, packet->start_1);
        printf(" \033[1;33mDST:\033[0m %2X |", packet->dst);
        printf(" \033[1;33mSRC:\033[0m %2X |", packet->src);
        printf(" \033[1;33mSIZE:\033[0m %2X |", packet->size);
        for(int i=0; i < packet->size; i++){
            printf(" \033[1;33mDATA %d:\033[0m %2X |", i, packet->payload[i]);
        }

        printf(" \033[1;33mCRC 1:\033[0m %2X |", packet->crc_1);
        printf(" \033[1;33mCRC 0:\033[0m %2X |", packet->crc_0);
        printf("\n\n");
    }
#endif /* DN_ERROR_VERBOSE */
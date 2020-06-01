#include <stdio.h>
#include "CRC.h"
#include <stdint.h>

#define BUFF_SIZE 32

int main(){
    crcInit();
    
    uint8_t buff[BUFF_SIZE];
    uint8_t i;
    for(i=0; i< BUFF_SIZE -2; i++){
        buff[i]=i;
    }
    crc_t crc = crcFast((unsigned char*) buff, BUFF_SIZE-2);
    buff[i++] = (uint8_t)(crc>>8);
	buff[i++] = (uint8_t)crc;

    printf("CRC first packet part = %d\n", crc);

    crc = crcFast((unsigned char*) buff, BUFF_SIZE);
    printf("CRC all packet = %d\n", crc); // msut be zero
}
#pragma once
#include <stdint.h>

#define DEBUG_UART


typedef struct Buffer_t{
    void* buffer;
    uint16_t size;
}Buffer_t;

extern Buffer_t RXBuffer;
extern Buffer_t TXBuffer;

int UART_init(unsigned long baud_rate);

//Read from UART and store into buffer pointer
//Using global RXBuffer for better performance
int UART_read(Buffer_t* buffer);

//Write buffer on UART
//Using global TXBuffer for better performance
int UART_write(Buffer_t* buffer);

int UART_close();

#ifdef DEBUG_UART
    void printBuffer(Buffer_t* buffer);
#endif
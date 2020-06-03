#pragma once
#include <stdint.h>

#define DEBUG_UART

#define PRINT_ERROR

typedef struct Buffer_t{
    void* buffer;
    uint16_t size;
    uint16_t max_size;
    uint16_t index;
}Buffer_t;

extern Buffer_t RXBuffer;
extern Buffer_t TXBuffer;

int UART_init(unsigned long baud_rate);

//Read from UART and store into buffer pointer
//Using global RXBuffer for better performance
//This function is not blocking!
int UART_read(Buffer_t* buffer);

//Read from UART and store into buffer pointer
//Using global RXBuffer for better performance
//waiting timeout_us if there is no data to read
//it subtracts waited_time to timeout_us
int UART_read_waiting(Buffer_t* buffer, double* timeout_us);

//Write buffer on UART
//Using global TXBuffer for better performance
int UART_write(Buffer_t* buffer);


int UART_close();

#ifdef DEBUG_UART
    void printBuffer(Buffer_t* buffer);
#endif
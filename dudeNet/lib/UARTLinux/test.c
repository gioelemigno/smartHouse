#include <stdio.h>
#include "UART.h"



int main(){
    uint8_t* tx = (uint8_t*)TXBuffer.buffer;

    tx[0] = 0xFF;
    tx[1] = 0x12;
    TXBuffer.size = 100;

    printBuffer(&TXBuffer);
    UART_init(115200);
    UART_write(&TXBuffer);
    UART_read(&RXBuffer);
    UART_close();
}
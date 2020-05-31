#include <stdio.h>
#include "UART.h"



int main(){
    uint8_t* tx = (uint8_t*)TXBuffer.buffer;

    tx[0] = 0xFF;
    tx[1] = 0x12;
    TXBuffer.size = 2;

    //printBuffer(&TXBuffer);
    UART_init(115200);
    while(1){
    usleep(100);
    UART_write(&TXBuffer);
    //usleep(100);
        int res = UART_read_waiting(&RXBuffer, 1E03);
        if(res == -1) printf("nothing to read :(\n");
    }
    UART_close();
}
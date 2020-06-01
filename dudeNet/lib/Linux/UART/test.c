#include <stdio.h>
#include "UART.h"
#include <stdlib.h>

#define TIMEOUT     10E03
int main(){
    uint8_t* tx = (uint8_t*)TXBuffer.buffer;

    tx[0] = 0xFF;
    tx[1] = 0x12;
    TXBuffer.size = 2;

    //printBuffer(&TXBuffer);
    int res = UART_init(115200);
    if(res == -1) exit(EXIT_FAILURE);
    while(1){
        usleep(100);
        UART_write(&TXBuffer);
        //usleep(100);
        double timeout = TIMEOUT;
        int res = UART_read_waiting(&RXBuffer, &timeout);
        printf("MAIN waited %.2f\n", (TIMEOUT - timeout));

        if(res == -1) printf("nothing to read :(\n");
    }
    res = UART_close();
    if(res == -1) exit(EXIT_FAILURE);

}
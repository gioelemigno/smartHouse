#include "UART.h"
#include <util/delay.h>

#define BUFF_SIZE 128
int main(){
    UART_init(115200);
    uint8_t buffer[BUFF_SIZE];

    for(uint8_t i = 0; i< BUFF_SIZE; i++){
        buffer[i] = i;
    }
    
    while(1){
        UART_write(buffer, BUFF_SIZE);
        while(!write_completed);
        UART_write(buffer, BUFF_SIZE);
        while(!write_completed);

        _delay_ms(1000);
    }
}
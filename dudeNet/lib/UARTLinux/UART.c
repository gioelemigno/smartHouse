#include "UART.h"
#include <termios.h>

#ifdef DEBUG_UART
    #include <stdio.h>
#endif


#define BUFFER_SIZE 1024


uint8_t buffTX[BUFFER_SIZE];
uint8_t buffRX[BUFFER_SIZE];

Buffer_t RXBuffer = {
    .buffer = buffRX,
    .size = 0
};

Buffer_t TXBuffer = {
    .buffer = buffTX,
    .size = 0
};

int UART_init(unsigned long baud_rate){
    speed_t myBaud;

    switch(baud_rate){  // code from WiringPi library
        case      50:	myBaud =      B50 ; break ;
        case      75:	myBaud =      B75 ; break ;
        case     110:	myBaud =     B110 ; break ;
        case     134:	myBaud =     B134 ; break ;
        case     150:	myBaud =     B150 ; break ;
        case     200:	myBaud =     B200 ; break ;
        case     300:	myBaud =     B300 ; break ;
        case     600:	myBaud =     B600 ; break ;
        case    1200:	myBaud =    B1200 ; break ;
        case    1800:	myBaud =    B1800 ; break ;
        case    2400:	myBaud =    B2400 ; break ;
        case    4800:	myBaud =    B4800 ; break ;
        case    9600:	myBaud =    B9600 ; break ;
        case   19200:	myBaud =   B19200 ; break ;
        case   38400:	myBaud =   B38400 ; break ;
        case   57600:	myBaud =   B57600 ; break ;
        case  115200:	myBaud =  B115200 ; break ;
        case  230400:	myBaud =  B230400 ; break ;
        case  460800:	myBaud =  B460800 ; break ;
        case  500000:	myBaud =  B500000 ; break ;
        case  576000:	myBaud =  B576000 ; break ;
        case  921600:	myBaud =  B921600 ; break ;
        case 1000000:	myBaud = B1000000 ; break ;
        case 1152000:	myBaud = B1152000 ; break ;
        case 1500000:	myBaud = B1500000 ; break ;
        case 2000000:	myBaud = B2000000 ; break ;
        case 2500000:	myBaud = B2500000 ; break ;
        case 3000000:	myBaud = B3000000 ; break ;
        case 3500000:	myBaud = B3500000 ; break ;
        case 4000000:	myBaud = B4000000 ; break ;

        default:    return -1; break;
    }

    return 0;
}

int UART_read(Buffer_t* buffer){
    return 0;
}

int UART_write(Buffer_t* buffer){
    return 0;
}

int UART_close(){
    return 0;
}

#ifdef DEBUG_UART
    void printBuffer(Buffer_t* buffer){
        uint8_t* buff = (uint8_t*) buffer->buffer;
        printf("Buffer: \n\tsize:\n\t\t%d\n\tpayload:\n\t\t", buffer->size);

        for(uint16_t i=0; i< buffer->size; i++){
            printf("%2X ", buff[i]);
            if(!((i+1) % 32)) printf("\n\t\t"); 
        }
        printf("\n");
    }
#endif
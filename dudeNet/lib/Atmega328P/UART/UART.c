#include "UART.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef RS485
    #include "../MAX485.h"
#endif
    
volatile uint8_t UART_write_completed;

uint8_t* TXBuffer = NULL;
uint16_t TXBuffer_size = 0;
volatile uint16_t TXBuffer_index = 0; 

void UART_init(unsigned long baud_rate){
    uint8_t oldSREG = SREG;
    cli();

    sbi(UCSR0A, U2X0);  //enable Double the USART Transmission Speed

    sbi(UCSR0B, RXEN0); //enable RX
    sbi(UCSR0B, TXEN0); //enable TX

    ///////   USELESS CODE    //////////////////////////////////////////////////
        //Asynchronous USART
          cbi(UCSR0C, UMSEL01);
          cbi(UCSR0C, UMSEL00);

        //Disable Parity Mode
          cbi(UCSR0C, UPM01);
          cbi(UCSR0C, UPM00);


      //    cbi(UCSR0C, USBS0);// 1 stop bit

    ////////////////////////////////////////////////////////////////////////////
    sbi(UCSR0C, USBS0);// 2 stop bit

    // set 8 bit data size
    cbi(UCSR0C, UCSZ02);
    sbi(UCSR0C, UCSZ01);
    sbi(UCSR0C, UCSZ00);

    //Set baud rate
    uint16_t ubrr = F_CPU/8/baud_rate-1;
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    SREG = oldSREG;

    //Enable USART RX and TX interrupt
    enable_RX_interrupt();
    enable_TX_interrupt();

    #ifdef RS485
        MAX485_init();
        MAX485_enableRX();
    #endif

    UART_write_completed = true;

    sei(); //enable all interrupt (set Bit 7 Global Interrupt Enable in SREG)
}

void UART_write(uint8_t* buffer, uint16_t size){
    #ifdef RS485
        MAX485_enableTX();
    #endif

    UART_write_completed = false;
    TXBuffer_index = 0;

    TXBuffer = buffer;
    TXBuffer_size = size;

    //first TX
    UDR0 = TXBuffer[TXBuffer_index++];
}

uint8_t UART_read(){
  // Wait for incoming data, looping on status bit
  while ( !(UCSR0A & (1<<RXC0)) );
  
  // Return the data
  return UDR0;
}


ISR(USART_TX_vect){
    if(TXBuffer_index == TXBuffer_size){ //TXComplete
        #ifdef RS485
            MAX485_enableRX();
        #endif
        UART_write_completed = true;
    }
    else{
        UDR0 = TXBuffer[TXBuffer_index++];
    }
}
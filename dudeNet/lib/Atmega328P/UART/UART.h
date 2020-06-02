#pragma once
#include <stdint.h>
#include "../avrMacro.h"

#define RS485


//Reference: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
// pag 160
#define enable_RX_interrupt() sbi(UCSR0B, RXCIE0)
#define disable_RX_interrupt() cbi(UCSR0B, RXCIE0)

#define enable_TX_interrupt() sbi(UCSR0B, TXCIE0)
#define disable_TX_interrupt() cbi(UCSR0B, TXCIE0)

extern volatile uint8_t UART_write_completed;

void UART_init(unsigned long baud_rate);

//send buffer using interrupt
//when write of buffer is complete, "write_completed" is setted
//NOTE: While sending data over UART, MUST DON'T TOUCH BUFFER, wait write_completed
void UART_write(uint8_t* buffer, uint16_t size);

//read UDR0 register
uint8_t UART_read();






#pragma once
#include "avrMacro.h"
#include <avr/io.h>

#define MAX485_enableTX() PORTB |= (1 << PB0) //digitalWRite(8, HIGH)
#define MAX485_enableRX() PORTB &= ~(1 << PB0) //digitalWRite(8, LOW)

#define MAX485_init() sbi(DDRB, PB0) //pinMode(8, OUTPUT)

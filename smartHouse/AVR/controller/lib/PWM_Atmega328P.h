#pragma once
//pwm code based on this example: https://gist.github.com/Wollw/2425784

#include <stdint.h>

//ONLY PIN 9 is supported so far!
int8_t PWM_Atmega328P_setup(uint8_t pin);

int8_t PWM_Atmega328P_analogWrite(uint8_t pin, uint8_t level);
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum ADC_pin{
  ADC_pin_A0=0,
  ADC_pin_A1,
  ADC_pin_A2,
  ADC_pin_A3,
  ADC_pin_A4,
  ADC_pin_A5,
  ADC_pin_A6,
  ADC_pin_A7
}ADC_pin;

void ADC_Atmega328P_init();
uint16_t ADC_Atmega328P_read(uint8_t pinADC);


#ifdef __cplusplus
}
#endif


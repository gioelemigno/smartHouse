#include "controller.h"
#include "lib/ADC_Atmega328P.h"
#include "lib/Time_Atmega328.h"
#include <avr/interrupt.h>
#include <avr/io.h>

//adc read <= ADC_MIDDLE - THRESOLD => decrement
//adc read >= ADC_MIDDLE + THRESOLD => increment
#define THRESOLD    200

#define ADC_MIDDLE      512

uint8_t analog_pin = 0;

void controller_init(uint8_t analogInput){
    ADC_Atmega328P_init();
    Time_Atmega328_init();
}

//return 1 to increment
//return -1 to decrement
// else return 0
int8_t controller_read(){
    uint8_t oldSREG =SREG;
    cli();
    uint16_t read = ADC_Atmega328P_read(analog_pin);
    SREG = oldSREG;

    if(read <= ADC_MIDDLE-THRESOLD)  return -1;
    else if(read >= ADC_MIDDLE + THRESOLD) return 1;
    else return 0;
}
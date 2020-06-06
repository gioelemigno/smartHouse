#include "controller.h"
#include "lib/ADC_Atmega328P.h"
#include "lib/Time_Atmega328.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "lib/avrMacro.h"

//adc read <= ADC_MIDDLE - THRESOLD => increment
//adc read >= ADC_MIDDLE + THRESOLD => decrement
#define THRESOLD    200

#define ADC_MIDDLE      512

//time to keep button pressed to toggle status
#define PIN_PRESSED_TIME_ms    100

uint8_t analog_pin = 0;

void controller_init(uint8_t analogInput){
    ADC_Atmega328P_init();
    Time_Atmega328_init();

    cbi(DDRD, PD2); //set pin 2 as input
    sbi(PORTD, PD2); //enable pull-up resistor

}

//return 1 to increment
//return -1 to decrement
// else return 0
int8_t controller_read(){
    uint8_t oldSREG =SREG;
    cli();
    uint16_t read = ADC_Atmega328P_read(analog_pin);
    SREG = oldSREG;

    if(read <= ADC_MIDDLE-THRESOLD)  return 1;
    else if(read >= ADC_MIDDLE + THRESOLD) return -1;
    else if( !rbi(PIND, PD2)){ //button pressed
        unsigned long time_start = Time_Atmega328_millis(); 
        while(Time_Atmega328_millis()- time_start < PIN_PRESSED_TIME_ms && !rbi(PIND, PD2));
        if(!rbi(PIND, PD2)) return 2;   //toggle status
        else return 0;
    }
    else return 0;
}
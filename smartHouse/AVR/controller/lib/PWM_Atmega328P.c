//pwm code based on this example: https://gist.github.com/Wollw/2425784
#include "PWM_Atmega328P.h"
#include "avrMacro.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
int8_t PWM_Atmega328P_setup(uint8_t pin){
    uint8_t oldSREG = SREG;
    switch (pin){
        case 9:  
            cli();
            
            sbi(DDRB, PB1); //set as output
            
            //pag 109: setting mode 5 (Fast PWM 8bit)
            sbi(TCCR1A, WGM10);
            sbi(TCCR1B, WGM12);

            //pag 108: setting(PWM Mode) :Clear OC1A/OC1B on compare match, set OC1A/OC1B a BOTTOM (non-inverting mode)
            sbi(TCCR1A, COM1A1);
                cbi(TCCR1A, COM1A0); //useless it's default value

            //pag 110: clkI/O/1 (no prescaling)
                cbi(TCCR1B, CS12);//useless it's default value
                cbi(TCCR1B, CS11);//useless it's default value
            sbi(TCCR1B, CS10);

            //source code:
            //TCCR1A |= _BV(COM1A1) | _BV(WGM10);
            //TCCR1B |= _BV(CS10) | _BV(WGM12);
        
            SREG = oldSREG;
            return 0;
        break;
        
        default:
            return -1;
        break;
    }
}

int8_t PWM_Atmega328P_analogWrite(uint8_t pin, uint8_t level){
    uint8_t oldSREG = SREG;
    switch (pin){
        case 9:
            if(level == 255) {
                cli();

                cbi(TCCR1A, COM1A1); //poweroff pwm on pin
                sbi(PORTB, PB1);

                SREG=oldSREG;
            }
            else if(level == 0) {
                cli();
                
                //avoid residual voltage cause by high frequency output
                cbi(TCCR1A, COM1A1); //poweroff pwm on pin
                cbi(PORTB, PB1);
                
                SREG=oldSREG;
            }
            else {
                cli();

                sbi(TCCR1A, COM1A1); //poweron pwm on pin
                OCR1A = level;
                
                SREG=oldSREG;
            }
            return 0;
        break;
        
        default:
            return -1;
        break;
    }
}
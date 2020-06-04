#include "ADC_Atmega328P.h"

#include "avrMacro.h"
#include <avr/io.h>
#include <util/delay.h>

void ADC_Atmega328P_init(){

  //Voltage Reference Selection: AVCC with external capacitor at AREF pin
  //  it's DEFAULT reference in Arduino
  cbi(ADMUX, REFS1);
  sbi(ADMUX, REFS0);

  cbi(ADMUX, ADLAR); //LSB in ADCL and MSB in ADCH (LSB and MSB of 10bit result)

  //ADC Prescaler Selections: Division Factor = 128 (16MHz/128)=125kHz
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  sbi(ADCSRA, ADEN);  //enable ADC
}

uint16_t ADC_Atmega328P_read(uint8_t pinADC){
  ADMUX &= 0xF0;  //reset MUX3 ... MUX0 (pin select)

  // select new pinADC
  ADMUX |= (pinADC & 0x0F);  //0x07 avoid write of 5 MSB in ADMUX

  //printf("ADMUX: %x\n", ADMUX);

  sbi(ADCSRA, ADSC); //start conversion

  while(rbi(ADCSRA, ADSC)); //waiting conversion end

/*      SEE pag 265 atmega328P datasheet
*When ADCL is read, the ADC Data Register is not updated until ADCH is read. Consequently, if
*the result is left adjusted and no more than 8-bit precision is required, it is sufficient to read
*ADCH. Otherwise, ADCL must be read first, then ADCH.
*/
  uint8_t low = ADCL;
  uint8_t high = ADCH;

  return (high << 8) | low;
}

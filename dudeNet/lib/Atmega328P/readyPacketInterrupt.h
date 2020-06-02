#pragma once
#include "avrMacro.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define readyPacketInterrupt_throwINT() \
  if(rbi(PORTD, PD4)) cbi(PORTD, PD4); \
  else sbi(PORTD, PD4)

/*
  sbi(DDRD, PD4); \ //set pin 4 as output
  sbi(PCMSK2, PCINT20); //enable PICINT20 (pin4) interrupt

  sbi(PCICR, PCIE2); //enable PCINT23..16 pin interrupt
  sei(); //enable all interrupt
*/
void readyPacketInterrupt_init();

#define readyPacketInterrupt_enable() \
  sbi(PCMSK2, PCINT20)

#define readyPacketInterrupt_disable()  \
  cbi(PCMSK2, PCINT20)
  
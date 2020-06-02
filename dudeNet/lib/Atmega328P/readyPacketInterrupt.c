#include "readyPacketInterrupt.h"
#include "avrMacro.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../../DNRouting.h"

unsigned char oldPORTD = 0; //in ISR we can determinate pin changed

void readyPacketInterrupt_init() {
  sbi(DDRD, PD4); //set pin 4 as output
  sbi(PCMSK2, PCINT20);  //enable PICINT20 (pin4) interrupt
  sbi(PCICR, PCIE2); //enable PCINT23..16 pin interrupt

  oldPORTD = PORTD; //in ISR we can determinate which pin changed
}

ISR(PCINT2_vect){

  unsigned char pins_changed = oldPORTD ^ PORTD;
  oldPORTD = PORTD;

  /*
  * HANDLER PACKET READY INTERRUPT
  */
  if(rbi(pins_changed, PD4)){  //execute if only pin 4 changed state
    //disable_packet_ready_interrupt();

    readyPacketInterrupt_disable(); //avoid nested same interrupt

    //SREG will restored after ISR with reti istruction
    sei();  //enable nested interrupt

    packetHandler(&packetRX);//  netHandler();

    readyPacketInterrupt_enable();
    //enable_packet_ready_interrupt();
  }

}
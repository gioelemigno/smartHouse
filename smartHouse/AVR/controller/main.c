#include "../../../dudeNet/dudeNet.h"

#include "lib/ADC_Atmega328P.h"
#include "lib/Time_Atmega328.h"
#include "controller.h"
#include <stdint.h>

#include "../../packet.h"

volatile int8_t controller_command = NONE;

void netHandler(volatile packet_t* packet);


int main(){
    DNRouting_init(115200);
    myAddress = 0xA0;

    #ifdef ADDRESS
        myAddress = ADDRESS;
    #endif
    
    packetHandler = netHandler;
    //ADC_Atmega328P_init();
    Time_Atmega328_init();

    controller_init(0); //A0

    while(1){
        controller_command=controller_read();
        
        //avoid loop toogle_status when button pressed
        if(controller_command == TOGGLE_STATUS){
            while(controller_read() == TOGGLE_STATUS);
            continue;
        }
        Time_Atmega328_delay_ms(150); //smart delay
    }

}

void netHandler(volatile packet_t* __packet){


    application_packet_t* packet_rx = (application_packet_t*) (&(__packet->dst)); 
    
    application_packet_t* packet_tx = (application_packet_t*) (&(packetTX.dst)); 
    
    switch (packet_rx->command){
        case READ:
            packet_tx->dst = packet_rx->src;
            packet_tx->size=1;

            packet_tx->command =controller_command;
            /*
            switch (controller_command){
                case 1:
                    packet_tx->command = INCREMENT;
                break;
            
                case -1:
                    packet_tx->command = DECREMENT;
                break;

                case 2:
                    packet_tx->command = TOGGLE_STATUS;
                break;

                default:
                    packet_tx->command = NONE;
                break;
            }
            */
            DNRouting_write(&packetTX);
            controller_command=NONE;//0;
        break;
    
        default:
            return;
            /*
            packet_tx->dst = packet_rx->src;
            packet_tx->size=1;
            switch (controller_command){
                case 1:
                    packet_tx->command = INCREMENT;
                break;
            
                case -1:
                    packet_tx->command = DECREMENT;
                break;

                default:
                    packet_tx->command = NONE;
                break;
            }
            DNRouting_write(&packetTX);
            controller_command=0;
            */
        break;
    }
}
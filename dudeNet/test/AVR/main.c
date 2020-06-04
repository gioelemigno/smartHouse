#include "../../dudeNet.h"
#include <util/delay.h>

void netHandler(volatile packet_t* packet){
    packetRX.dst=packetRX.src;
    DNRouting_write(&packetRX);
}

int main(){
    DNRouting_init(115200);
    myAddress = 0xA0;

    #ifdef ADDRESS
        myAddress = ADDRESS;
    #endif
    
    packetHandler = netHandler;

    /*
    packetTX.start_0 =START_0;
    packetTX.start_1 =START_1;
    */
    packetTX.dst = 0x10;

    packetTX.size = 2;
    packetTX.payload[0] = 0x23;
    packetTX.payload[1] = 0x12;

    while(1){
        //DNRouting_write(&packetTX);
        _delay_ms(1000);        
    }
}
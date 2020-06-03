#include "../../dudeNet.h"
#include <stdio.h>
#include <unistd.h>
int main(){
    DNRouting_init(115200);
    myAddress = 0xA0;

    packetTX.dst = 0xA0;
    packetTX.size = 2;
    packetTX.payload[0] = 0x13;
    packetTX.payload[1] = 0x9A;

    while(1){
        DNRouting_printPacket("TX", &packetTX);
        res_t res = DNRouting_write(&packetTX);
        if(res == -1) printf("errore write\n");

        res = DNRouting_read(&packetRX);
        if(res == -1) DNError_infoError(__func__);
        else DNRouting_printPacket("RX", &packetRX);
        sleep(1);
    }
}
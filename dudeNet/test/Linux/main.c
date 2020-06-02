#include "../../dudeNet.h"

int main(){
    DNRouting_init(115200);
    myAddress = 0x1F;

    packetTX.dst = 0xA0;
    packetTX.size = 2;
    packetTX.payload[0] = 0x13;
    packetTX.payload[1] = 0x9A;

    while(1){
        DNRouting_write(&packetTX);

        DNRouting_read(&packetRX);
        sleep(1);
    }
}
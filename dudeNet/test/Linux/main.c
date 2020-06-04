#include "../../dudeNet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sigint_handler(int sig){
	exit(EXIT_SUCCESS);
}
struct sigaction act = {0};


void sigint_handler_init(){
	act.sa_handler = sigint_handler;
	if(sigaction(SIGINT, &act, NULL) ==-1){
		perror("sigaction error");
		exit(EXIT_FAILURE);
	}
}



int main(){
    sigint_handler_init();
    atexit(&dudeNet_atexit);

    DNRouting_init(115200);
    myAddress = 0xA0;

    packetTX.dst = 0x2;//0xF1; //0xA0;
    packetTX.size = 2;
    packetTX.payload[0] = 0x13;
    packetTX.payload[1] = 0x9A;

    while(1){
        packetRX.dst=0x2;
        DNRouting_printPacket("TX->2", &packetTX);
        res_t res = DNRouting_write(&packetTX);
        if(res == -1) printf("errore write\n");

        res = DNRouting_read(&packetRX);
        if(res == -1) DNError_infoError(__func__);
        else DNRouting_printPacket("2->RX", &packetRX);

        packetTX.dst=0x2;
        DNRouting_printPacket("TX->3", &packetTX);
        res = DNRouting_write(&packetTX);
        if(res == -1) printf("errore write\n");

        res = DNRouting_read(&packetRX);
        if(res == -1) DNError_infoError(__func__);
        else DNRouting_printPacket("3-> RX", &packetRX);
        sleep(1);
    }
}
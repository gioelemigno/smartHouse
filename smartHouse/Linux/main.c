#include "../../dudeNet/dudeNet.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../packet.h"

#define ADDRESS_CONTROLLER  0x2
#define ADDRESS_LED         0x3

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

    application_packet_t* packet_rx = (application_packet_t*) (&(packetRX.dst)); 
    
    application_packet_t* packet_tx = (application_packet_t*) (&(packetTX.dst)); 
    res_t res = 0;
    while(1){
        packet_tx->dst=ADDRESS_CONTROLLER;
        packet_tx->size=1;
        packet_tx->command=READ;

        DNRouting_printPacket("WRITE", &packetTX);

        res = DNRouting_write(&packetTX);
        if(res == -1)   {
            DNError_infoError(__func__);
            continue;//ecit(EXIT_FAILURE);
        }
        res = DNRouting_read(&packetRX);
        if(res == -1) {
            DNError_infoError(__func__);
            continue;
           // exit(EXIT_FAILURE);
        }

        switch (packet_rx->command){
            case INCREMENT:
                printf("increment\n");
            break;
            
            case DECREMENT:
                printf("decrement\n");
            break;
            
            default:
                printf("unknwon\n");
                break;
        }

        sleep(1);
    }
}
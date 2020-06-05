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

static inline int16_t read_controller();
static inline int16_t send_command_to_led(uint8_t command);

int main(){
    sigint_handler_init();
    atexit(&dudeNet_atexit);

    DNRouting_init(115200);
    myAddress = 0xA0;    


    while(1){

        int16_t res = read_controller();
        if(res==-1){
            printf("Error read controller\n");
            continue;
        }

        sleep(1);
    }
}

static inline int16_t read_controller(){

    application_packet_t* packet_rx = (application_packet_t*) (&(packetRX.dst)); 
    
    application_packet_t* packet_tx = (application_packet_t*) (&(packetTX.dst)); 
    res_t res = 0;
        
    packet_tx->dst=ADDRESS_CONTROLLER;
    packet_tx->size=1;
    packet_tx->command=READ;

    DNRouting_printPacket("WRITE", &packetTX);

    res = DNRouting_write(&packetTX);
    if(res == -1)   {
        DNError_infoError(__func__);
        return -1;//ecit(EXIT_FAILURE);
    }
    res = DNRouting_read(&packetRX);
    if(res == -1) {
        DNError_infoError(__func__);
        return -1;
       // exit(EXIT_FAILURE);
    }

    switch (packet_rx->command){
        case INCREMENT:
            printf("increment\n");
            return INCREMENT;
        break;
            
        case DECREMENT:
            printf("decrement\n");
            return DECREMENT;
        break;
            
        case NONE:
            printf("No command\n");
            return NONE;
        break;

        case TOGGLE_STATUS:
            printf("Toogle status\n");
            return TOGGLE_STATUS;
        break;

        default:
            printf("unknwon\n");
            return -1;
        break;
    }
    return -1;
}

static inline int16_t send_command_to_led(uint8_t command){

}

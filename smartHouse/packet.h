#pragma once

//#define MAX_SIZE_APPLICATION_PACKET     32-1 // 1= command
/*
typedef struct app_packet_t{
    uint8_t dst;
    uint8_t src;
    uint8_t size;
    
    uint8_t command; 
    uint8_t payload[MAX_SIZE_APPLICATION_PACKET];
}app_packet_t;
*/
typedef struct application_packet_t{
    uint8_t dst;
    uint8_t src;
    uint8_t size;
    
    uint8_t command; 
}application_packet_t;


////////////// COMMAND ///////////////////

//used with pwm led
    #define INCREMENT       127
    #define DECREMENT       126

    #define SET_ON          125
    #define SET_OFF         124

    #define NONE            123 
    #define TOGGLE_STATUS   122     

    #define ACK             121
//used with controller
    #define READ            64

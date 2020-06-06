
#include "../../../dudeNet/dudeNet.h"
#include "lib/PWM_Atmega328P.h"
#include "lib/Time_Atmega328.h"
#include "EEPROMLed.h"

#include <stdint.h>
#include "../../packet.h"

//15 brightness levels (255/15)
#define LED_BRIGHTNESS_STEP		17 

#define PIN_LED 	9

//#define ADDRESS_LED         0x3

void netHandler(volatile packet_t* __packet);

volatile uint8_t led_brightness = 0;
volatile uint8_t led_status = 0;

//use to reduce EEPROM Write
#define EEPROME_TIME_DELAY_ms	5E03	//5sec			
uint8_t old_led_brightness;
uint8_t old_led_status;
unsigned long time_start;


int main(){
    DNRouting_init(115200);

    #ifdef ADDRESS
        myAddress = ADDRESS;
    #endif
    myAddress = 0x3;

    packetHandler = netHandler;
    Time_Atmega328_init();

	PWM_Atmega328P_setup(PIN_LED);

	led_status = EEPROMLed_readStatus();
	led_brightness = EEPROMLed_readBrightness();

	old_led_status = led_status;
	old_led_brightness = led_brightness;
	
	if(led_status) PWM_Atmega328P_analogWrite(PIN_LED, led_brightness);
	else PWM_Atmega328P_analogWrite(PIN_LED, 0);
	
	time_start = Time_Atmega328_millis();

	while(1){
		if(old_led_brightness != led_brightness || old_led_status != led_status){
			if(Time_Atmega328_millis() - time_start >= EEPROME_TIME_DELAY_ms){
				EEPROMLed_writeStatus(led_status);
				old_led_status = led_status;

				EEPROMLed_writeBrightness(led_brightness);
				old_led_brightness=led_brightness;

				time_start = Time_Atmega328_millis();
			}
		}
	}
}

void netHandler(volatile packet_t* __packet){
	application_packet_t* packet_rx = (application_packet_t*) (&(__packet->dst)); 
    
    application_packet_t* packet_tx = (application_packet_t*) (&(packetTX.dst)); 
    
    switch (packet_rx->command){
        case INCREMENT:
			led_status = 1;
			if(led_brightness!=255)	led_brightness += LED_BRIGHTNESS_STEP;
			PWM_Atmega328P_analogWrite(PIN_LED, led_brightness);
        break;
	
		case DECREMENT:
			if(led_brightness){
				led_brightness -= LED_BRIGHTNESS_STEP;
				PWM_Atmega328P_analogWrite(PIN_LED, led_brightness);
			}	
		break;

		case TOGGLE_STATUS:
			led_status = !led_status;
			if(led_status) PWM_Atmega328P_analogWrite(PIN_LED, led_brightness);
			else PWM_Atmega328P_analogWrite(PIN_LED, 0);
		break;

        default:
			return;
        break;
    }
	packet_tx->dst = packet_rx->src;
    packet_tx->size=1;
	packet_tx->command=ACK;
	DNRouting_write(&packetTX);
}

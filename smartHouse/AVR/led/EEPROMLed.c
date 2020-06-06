#include "EEPROMLed.h"
#include <stdint.h>
#include <avr/eeprom.h> //https://www.nongnu.org/avr-libc/user-manual/group__avr__eeprom.html

#define EEPROM_SIZE     1024

#define EEPROM_LED_STATUS_INDEX         120
#define EEPROM_LED_BRIGHTNESS_INDEX     121

uint8_t EEPROMLed_readStatus(){
    return eeprom_read_byte((uint8_t*)EEPROM_LED_STATUS_INDEX);
}

uint8_t EEPROMLed_readBrightness(){
    return eeprom_read_byte((uint8_t*)EEPROM_LED_BRIGHTNESS_INDEX);
}

void EEPROMLed_writeStatus(uint8_t status){
    //avoid useless write of same value
    eeprom_update_byte((uint8_t*)EEPROM_LED_STATUS_INDEX, status);
}

void EEPROMLed_writeBrightness(uint8_t birghtness){
    eeprom_update_byte((uint8_t*)EEPROM_LED_BRIGHTNESS_INDEX, birghtness);
}
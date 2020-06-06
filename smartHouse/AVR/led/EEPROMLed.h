#pragma once
#include <stdint.h>


uint8_t EEPROMLed_readStatus();
uint8_t EEPROMLed_readBrightness();

void EEPROMLed_writeStatus(uint8_t status);
void EEPROMLed_writeBrightness(uint8_t birghtness);
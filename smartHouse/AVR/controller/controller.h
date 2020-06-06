#pragma once
#include <stdint.h>

void controller_init(uint8_t analogInput);

//read command from controller
int8_t controller_read();
#pragma once
#include <stdint.h>

void controller_init(uint8_t analogInput);

//return 1 to increment
//return -1 to decrement
int8_t controller_read();
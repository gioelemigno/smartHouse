#pragma once
#include <stdint.h>

void controller_init(uint8_t analogInput);

//return 1 to increment
//return -1 to decrement
// retrun 0 no command
// return 2 to toggle status
int8_t controller_read();
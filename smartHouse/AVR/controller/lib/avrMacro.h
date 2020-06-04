#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#define sbi(port,bit) \
	(port) |= (1 << (bit))

#define cbi(port,bit) \
	(port) &= ~(1 << (bit))

#define rbi(port,bit) \
	( (port) & (1 << (bit)) )

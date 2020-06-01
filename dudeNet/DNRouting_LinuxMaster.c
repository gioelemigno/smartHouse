#include "DNRouting.h"

address_t myAddress = NO_ADDRESS;

//Used by read function
volatile packet_t packetRX;

//used by write function
packet_t packetTX;
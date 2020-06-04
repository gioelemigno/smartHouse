#pragma once

/*******************************************************************************
*
*   Based on wiring.c file of Arduino core
*   It supports only Atmega328p
*
*******************************************************************************/
/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/


#ifdef __cplusplus
extern "C"{
#endif

void Time_Atmega328_init();

unsigned long Time_Atmega328_millis();
unsigned long Time_Atmega328_micros();

void Time_Atmega328_delay_ms(unsigned long timer);
void Time_Atmega328_delay_us(unsigned long timer);

#ifdef __cplusplus
}
#endif



AVR_CC=avr-gcc
AVR_CFLAGS+=-Os -g -DF_CPU=16000000UL -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p

AVR_LIB_NAME=libDudeNetAVR

$(AVR_LIB_NAME).a: DNError.o DNRouting_AtmegaSlave.o CRC.o readyPacketInterrupt.o UART.o
	ar rvs $(AVR_LIB_NAME).a $^

DNError.o: ../../DNError.c ../../DNError.h 
	$(AVR_CC) $(AVR_CFLAGS) $(DEFINE_FLAGS) -c  ../../DNError.c 
 
DNRouting_AtmegaSlave.o: ../../DNRouting_AtmegaSlave.c ../../DNRouting.h 
	$(AVR_CC) $(AVR_CFLAGS) $(DEFINE_FLAGS) -c  ../../DNRouting_AtmegaSlave.c 

readyPacketInterrupt.o: ../../lib/Atmega328P/readyPacketInterrupt.c ../../lib/Atmega328P/readyPacketInterrupt.h 
	$(AVR_CC) $(AVR_CFLAGS) $(DEFINE_FLAGS) -c  $<

CRC.o: ../../lib/CRC/CRC.c ../../lib/CRC/CRC.h
	$(AVR_CC) $(AVR_CFLAGS) $(DEFINE_FLAGS) -c  $<


UART.o: ../../lib/Atmega328P/UART/UART.c ../../lib/Atmega328P/UART/UART.h ../../lib/Atmega328P/avrMacro.h ../../lib/Atmega328P/MAX485.h
	$(AVR_CC) $(AVR_CFLAGS) $(DEFINE_FLAGS) -c  $< 



.phony: clean

clean:
	rm -f  *.o *.a


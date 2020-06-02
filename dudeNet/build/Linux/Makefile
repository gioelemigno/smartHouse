
CC=gcc
CFLAGS+=-g 

LINUX_LIB_NAME=libDudeNetLINUX

$(LINUX_LIB_NAME).a: DNError.o DNRouting_LinuxMaster.o CRC.o UART.o
	ar rvs $(LINUX_LIB_NAME).a $^

DNError.o: ../../DNError.c ../../DNError.h 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 
 
DNRouting_LinuxMaster.o: ../../DNRouting_LinuxMaster.c ../../DNRouting.h 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c $< 

readyPacketInterrupt.o: ../../lib/Atmega328P/readyPacketInterrupt.c ../../lib/Atmega328P/readyPacketInterrupt.h 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $<

CRC.o: ../../lib/CRC/CRC.c ../../lib/CRC/CRC.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $<


UART.o: ../../lib/Linux/UART/UART.c ../../lib/Linux/UART/UART.h 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 



.phony: clean

clean:
	rm -f  *.o *.a


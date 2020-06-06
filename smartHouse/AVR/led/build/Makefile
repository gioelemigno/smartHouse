
AVR_LIB_NAME=libDudeNetAVR


CC=avr-gcc
#CFLAGS=--std=gnu99 -O3 -DF_CPU=16000000UL  -funsigned-char -funsigned-bitfields  -fshort-enums -Wall -Wstrict-prototypes -mmcu=atmega328p $(INCLUDE_DIRS)  -D__AVR_3_BYTE_PC__
#CFLAGS=-O3 -DF_CPU=16000000UL -Wall -mmcu=atmega328p

CFLAGS+=-Os -g -DF_CPU=16000000UL -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p
#CFLAGS+=-DUID=0xFFF2
#CFLAGS+=-w -Os -g -DF_CPU=16000000UL -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega328p


#DEFINE_FLAGS=-D__arduino__

AVRDUDE=avrdude

AVRDUDE_PROGRAMMER=arduino
#AVRDUDE_PROGRAMMER=wiring

#avrdude -Pusb -v -p atmega328p -C /home/gio/.platformio/packages/tool-avrdude/avrdude.conf -c usbasp -D -U f
#lash:w:.pio/build/uno/firmware.hex:i

#AVRDUDE_PORT = /dev/ttyACM0    # programmer connected to serial device
AVRDUDE_PORT = /dev/ttyUSB0

	#UNO
AVRDUDE_FLAGS = -p m328p -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b 115200

	#PROMINI
#AVRDUDE_FLAGS = -p m328p -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b 57600

#AVRDUDE_FLAGS =-Pusb -v -p m328p -c usbasp -u -b 115200

AVRDUDE_FLAGS += -D -V -v -v -v  -C avrdude.conf


#AVRDUDE_FLAGS += -D -V -C /usr/share/arduino/hardware/tools/avr/../avrdude.conf

#AVRDUDE_FLAGS += -D -q -V -C /usr/share/arduino/hardware/tools/avr/../avrdude.conf
FIRMWARE = main

DEFINE_FLAGS=-DADDRESS=0x3

$(FIRMWARE).elf: ../main.c  EEPROMLed.o Time_Atmega328.o ADC_Atmega328P.o PWM_Atmega328P.o
	$(CC) $(CFLAGS) $(LIBS) $(DEFINE_FLAGS) -o $(FIRMWARE).elf  $^ ../../../../dudeNet/build/AVR/libDudeNetAVR.a
	#$(CC) $(CFLAGS) $(LIBS) $(DEFINE_FLAGS) -o $(FIRMWARE).elf  main.c ../../build/AVR/libDudeNetAVR.a
	avr-size -C --mcu=atmega328p main.elf

EEPROMLed.o : ../EEPROMLed.c ../EEPROMLed.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 

#controller.o: ../controller.c ../controller.h ADC_Atmega328P.o
#	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 
PWM_Atmega328P.o: ../lib/PWM_Atmega328P.c ../lib/PWM_Atmega328P.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 

Time_Atmega328.o: ../lib/Time_Atmega328.c ../lib/Time_Atmega328.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 

ADC_Atmega328P.o: ../lib/ADC_Atmega328P.c ../lib/ADC_Atmega328P.h 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  $< 

.phony: clean

clean:
	rm -f $(FIRMWARE).elf $(FIRMWARE).hex *.o *.a

.phony: upload

upload: $(FIRMWARE).elf
	avr-objcopy -O ihex -R .eeprom $(FIRMWARE).elf $(FIRMWARE).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$(FIRMWARE).hex:i #$(AVRDUDE_WRITE_EEPROM)
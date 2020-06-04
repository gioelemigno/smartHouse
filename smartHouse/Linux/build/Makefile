
CC=gcc
CFLAGS+=-g 


main: ../main.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -o main  ../main.c ../../../dudeNet/build/Linux/libDudeNetLINUX.a

.phony: clean

clean:
	rm -f  *.o *.a main


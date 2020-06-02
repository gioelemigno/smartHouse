
CC=gcc
CFLAGS+=-g 


main: main.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -o main  main.c ../../build/Linux/libDudeNetLINUX.a

.phony: clean

clean:
	rm -f  *.o *.a main


CC=gcc
CFLAGS=-g -c

all: main

main: main.o myqueue.o
	$(CC) -o main main.o myqueue.o
	
main.o: main.c
	$(CC) $(CFLAGS) -o main.o main.c

myqueue.o: myqueue.c myqueue.h
	$(CC) $(CFLAGS) -o myqueue.o myqueue.c

clean:
	rm *.o
	rm main

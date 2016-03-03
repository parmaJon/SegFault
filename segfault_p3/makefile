CC=gcc
CFLAGS=-g -c

all: main

main: main.o myqueue.o
	$(CC) -pthread main.o myqueue.o -o main
	
main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

myqueue.o: myqueue.c myqueue.h
	$(CC) $(CFLAGS) myqueue.c -o myqueue.o

clean:
	rm *.o
	rm main

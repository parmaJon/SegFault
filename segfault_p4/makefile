CC=gcc
CFLAGS=-g -c

all: scheduler

scheduler: main.o myqueue.o
	$(CC) main.o myqueue.o -o scheduler
	
main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

myqueue.o: myqueue.c myqueue.h
	$(CC) $(CFLAGS) myqueue.c -o myqueue.o

clean:
	rm *.o
	rm scheduler

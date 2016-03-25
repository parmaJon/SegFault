CC=gcc
CFLAGS=-g -c

all: scheduler

scheduler: main.o myqueue.o functions.o
	$(CC) main.o myqueue.o functions.o -o scheduler
	
main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o

myqueue.o: myqueue.c myqueue.h
	$(CC) $(CFLAGS) myqueue.c -o myqueue.o

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) functions.c -o functions.o 

clean:
	rm *.o
	rm scheduler

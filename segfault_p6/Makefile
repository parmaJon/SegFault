CC = gcc
CCOPTS = -c -g -Wall
LINKOPTS = -g -lrt -pthread

EXEC=banana
OBJECTS=disk.o main.o

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LINKOPTS) -o $@ $^ -lm

%.o:%.c
	$(CC) $(CCOPTS) -o $@ $^

clean:
	- $(RM) $(EXEC)
	- $(RM) $(OBJECTS)
	- $(RM) *~
	- $(RM) core.*


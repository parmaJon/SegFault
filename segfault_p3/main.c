/******************************************************
 * Filename: main.c
 *
 * Description: This program utilizes pthreads and a
 * queue to simulate a producer/consumer relationship
 * handling various queueing paradigms
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant, Garrett Ransom,
 *     Jonathan Brandt, Travis Nordquist
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #3
 * Assigned: Feb. 19, 2016
 * Due: Mar. 4, 2016
******************************************************/

#include "myqueue.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

int rand_max;
static sem_t empty;
static sem_t full;
static sem_t mutex;


/**
 * @param str the string in question
 * @return 1 if string is a number, 0 otherwise
 */
int isNum(char *str) {
    
    char *s;
    if(str == NULL) {
        return 0;
    }
    strtod(str, &s);
    
    return *s == '\0';
}


int main(int argc, char *argv[]) {

    int args[3];
    int i;


    /* Get command line arguments */
    if(argc != 4) {
        perror("ERROR: Wrong number of arguments\nUSAGE: ./main [num_producers] [num_consumers] [order_id]\n");
        return -1;
    }

    for(i = 1; i < 4; i++) {
        if(!isNum(argv[i])) {
            perror("ERROR: Argument is not an integer\n");
            return -1;
        }
        args[i-1] = atoi(argv[i]);
    }
    
    rand_max = args[0] - 1;

    /* Initialize queue */
    myqueue.size = 0;
    myqueue.head = NULL;
    myqueue.tail = NULL;
    for(i = 0; i < MAX_PROCESSES; i++) {
        if(enqueue(-1) == FALSE) {
            perror("Queue initialization failed\n");
            return -1;
        }
    }
    
    /* Create Producer thread(s) */
    
    /* Create Consumer thread(s) */
    
    /* Sleep for 300 seconds */
    //sleep(3);
    
    /* Exit */
    return 0;
}

/**
 * Defines the behavior for a producer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
void * producer(void * arg) {
    return 0; 
}


/**
 * Defines the behavior for a consumer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
void * consumer(void * arg) {
    return 0; 
}


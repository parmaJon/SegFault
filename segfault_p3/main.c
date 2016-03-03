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

int rand_max;


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
    int i = 1;


    /* Get command line arguments */
    if(argc != 4) {
        perror("ERROR: Wrong number of arguments\nUSAGE: ./main [num_producers] [num_consumers] [order_id]\n");
        return -1;
    }

    for(; i < 4; i++) {
        if(!isNum(argv[i])) {
            perror("ERROR: Argument is not an integer\n");
            return -1;
        }
        args[i] = atoi(argv[i]);
    }
    
    rand_max = args[1] - 1;
    
    /* Initialize queue */
    myqueue.size = 0;
    myqueue.head = NULL;
    myqueue.tail = NULL;
    
    /* Create Producer thread(s) */
    
    /* Create Consumer thread(s) */
    
    /* Sleep for 300 seconds */
    sleep(300);
    
    /* Exit */
}

/**
 * Defines the behavior for a producer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
int producer(int threadID) {
    return 0; 
}


/**
 * Defines the behavior for a consumer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
int consumer(int threadID) {
    return 0; 
}


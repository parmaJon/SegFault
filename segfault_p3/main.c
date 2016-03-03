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
int args[3];
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

    int i;
    srand(time(NULL)); //set random seed

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

    if( args[2] > 2  ||  args[2] < 0 ) {
        printf("ERROR: Argument 3 must be either 0-FIFO, 1-RANDOM, or 2-TARGETED\n");
        return -1;
    }
    
    rand_max = args[1] - 1;

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

    /* Initialize semaphores */
    if (sem_init(&empty, 0, MAX_PROCESSES) == -1)
        perror("sem_init failed for empty\n");
    if (sem_init(&full, 0, 0) == -1)
        perror("sem_init failed for full\n");
    if (sem_init(&mutex, 0, 1) == -1)
        perror("sem_init failed for mutex\n");

    
    /* Create Producer thread(s) */
    
    /* Create Consumer thread(s) */
    
    /* Sleep for 300 seconds */
    sleep(3); //temporarily lowered
    
    /* Terminate all threads */

    /* Exit */
    clear();
    return 0;
}

/**
 * Defines the behavior for a producer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
void * producer(void * arg) {
    struct timespec* time = malloc(sizeof(struct timespec));
    time->tv_nsec = rand() % 1001;
    time->tv_sec = 0;

    /* wait for the random amount of time */
    nanosleep(time,time);

    int val = rand()%rand_max;
    
    sem_wait(&empty); //check if room in queue
    sem_wait(&mutex); //begin critical section

    if( enqueue(val) != -1 ) {
        printf("item (%d) added by Producer %d: queue = ", val, pthread_self());
        listQueue();
    }
    else {
        printf("Error: Producer %d could not add its value!\n", pthread_self());
    }

    sem_post(&mutex); //end critical section
    sem_post(&full);  //increment the count of queue elements

    free(time);
    return 0; 
}


/**
 * Defines the behavior for a consumer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
void * consumer(void * arg) {
    //thread_data_t *data = (thread_data_t *)arg;

    struct timespec* time = malloc(sizeof(struct timespec));
    time->tv_nsec = rand() % 1001;
    time->tv_sec = 0;

    /* wait for the random amount of time */
    nanosleep(time,time);

    wait(&full); //ensure the queue has an element
    wait(&mutex); //enter critical section

    switch(args[2]) {
    case 0:
        printf("item (%d) taken by Consumer %d: queue = ", dequeue(), pthread_self());
        break;
    case 1:
        printf("item (%d) taken by Consumer %d: queue = ", randomTarget(), pthread_self());
        break;
    case 2:
        printf("item (%d) taken by Consumer %d: queue = ", target(pthread_self()), pthread_self());
        break;
    default:
        printf("Consumer %d says - \"Wait...what am I supposed to do?\"", pthread_self());
    }

    sem_post(&mutex); //end critical section
    sem_post(&empty); //indicate that queue space is available

    free(time);
    return 0; 
}


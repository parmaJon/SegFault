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

void * producer(void * arg);
void * consumer(void * arg);

static int rand_max;
int args[3];
sem_t *empty, *full, *mutex;
bool cont_flag = TRUE;


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
    
    rand_max = args[1];

    /* Initialize queue */
    myqueue.size = 0;
    myqueue.head = NULL;
    myqueue.tail = NULL;

    mutex = malloc(sizeof(sem_t));
    full = malloc(sizeof(sem_t));
    empty = malloc(sizeof(sem_t));
    /* Initialize semaphores */
    sem_destroy(mutex);
    sem_destroy(full);
    sem_destroy(empty);
    if (sem_init(empty, 0, MAX_PROCESSES) < 0)
        perror("sem_init failed for empty\n");
    if (sem_init(full, 0, 0) < 0)
        perror("sem_init failed for full\n");
    if (sem_init(mutex, 0, 1) < 0)
        perror("sem_init failed for mutex\n");

    
    /* Create Producer thread(s) */
    
    pthread_t prodthreads[args[0]];  //stores producer thread IDs
    int rcp;     //stores the return code when creating threads (0 if no error)
    int t;       //thread count
    int* proargs[args[0]];

	/* loops to create # of threads determined by user */
    for(t=args[0]-1; t >= 0; t--){
      int *arg = malloc(sizeof(int));
      proargs[t] = arg;
      *arg = t;
      rcp=pthread_create(&prodthreads[t], NULL, producer, (void *)arg);  //returns error code printed below
      //pthread_detach(prodthreads[t]);
      if(rcp){
	  printf("ERROR producer creation; return code from pthread_create() is %d\n", rcp);
	  exit(-1);
      }
    }


    /* Create Consumer thread(s) */
    pthread_t conthreads[args[1]];
    int rcc;
    int* conargs[args[1]];

    for(t=args[1]-1; t >= 0; t--){
      int *arg = malloc(sizeof(int));
      conargs[t] = arg;
      *arg = t;
      rcc=pthread_create(&conthreads[t], NULL, consumer, (void *)arg);
      //pthread_detach(conthreads[t]);
      if(rcc){
	  printf("ERROR consumer creation; return code from pthread_create() is %d\n", rcc);
	  exit(-1);
      }

    }    


    /* Sleep for 300 seconds */
    sleep(5); //temporarily lowered
    
    /* Terminate all threads */
    cont_flag = FALSE;
    sleep(3);

    /* In cases where threads are stuck, increment semaphores to allow termination */
    i = args[0];
    while ( i > args[1] ) {
        sem_post(empty);
        i--;
    }
    i = args[1];
    while ( i > args[0] ) {
        sem_post(full);
        i--;
    }

    /* Await termination of all threads */
    for( t=0; t < args[0]; t++ ) {
        pthread_join(prodthreads[t],NULL);
    }
    for( t=0; t < args[1]; t++ ) {
        pthread_join(conthreads[t],NULL);
    }

    /* Exit */
    free(mutex);
    free(empty);
    free(full);
    clear();
    return 0;
}

/**
 * Defines the behavior for a producer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
void * producer(void * arg) {
  int *data = (int *)arg;

  while(cont_flag) {
    sleep(1);//tmp

    struct timespec* time = malloc(sizeof(struct timespec));
    time->tv_nsec = rand() % 1001;
    time->tv_sec = 0;

    /* wait for the random amount of time */
    nanosleep(time,time);

    int val = rand()%rand_max;
    
    sem_wait(empty); //check if room in queue
    //printf("Prod %d - Through empty\n", *data);
    sem_wait(mutex); //begin critical section
    //printf("Prod %d - Through lock\n", *data);

    /* Ensure that execution should still continue */
    if (cont_flag) {
        if( enqueue(val) != FALSE ) {
            printf("item (%d) added by Producer %d: queue = ", val, *data);
            listQueue();
        }
        else {
            printf("Error: Producer %d could not add its value!\n", *data);
        }
    }

    //printf("Prod %d - Opening lock\n", *data);
    sem_post(mutex); //end critical section
    //printf("Prod %d - Inc full\n", *data);
    sem_post(full);  //increment the count of queue elements

    free(time);    
  }

  printf("Prod %d - Terminating\n", *data);
  free(arg);
  return 0;
}


/**
 * Defines the behavior for a consumer thread
 * @param threadID - Integer thread ID of this thread
 * @return - Status, 0 if successful.
 */
void * consumer(void * arg) {
  int *data = (int *)arg;

  while(cont_flag) {
    sleep(1);//tmp

    struct timespec* time = malloc(sizeof(struct timespec));
    time->tv_nsec = rand() % 1001;
    time->tv_sec = 0;

    /* wait for the random amount of time */
    nanosleep(time,time);


    sem_wait(full); //ensure the queue has an element
    //printf("Con %d - Through full\n", *data);
    sem_wait(mutex); //enter critical section
    //printf("Con %d - Through lock\n", *data);


    /* Ensure that execution should still continue */
    if (cont_flag) {
        switch(args[2]) {
        case 0:
            printf("item (%d) taken by Consumer %d: queue = ", dequeue(), *data);
	    listQueue();
            break;
        case 1:
            printf("item (%d) taken by Consumer %d: queue = ", randomTarget(), *data);
	    listQueue();
            break;
        case 2:
            printf("item (%d) taken by Consumer %d: queue = ", target(*data), *data);
            listQueue();
            break;
        default:
            printf("Consumer %d says - \"Wait...what am I supposed to do?\"", *data);
        }
    }

    //printf("Con %d - Opening lock\n", *data);
    sem_post(mutex); //end critical section
    //printf("Con %d - Inc empty\n", *data);
    sem_post(empty); //indicate that queue space is available

    free(time);
  }

  printf("Con %d - Terminating\n", *data);
  free(arg);
  return 0; 
}


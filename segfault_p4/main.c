/******************************************************
 * Filename: main.c
 *
 * Description: This program simulates scheduling
 * algorithms by queueing and using process information
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant, Garrett Ransom,
 *     Jonathan Brandt, Travis Nordquist
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #4
 * Assigned: Mar. 4, 2016
 * Due: Mar. 25, 2016
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
int time_quantum;


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
    char buf[33];
    FILE *in;
    Queue toArrive;
    srand(time(NULL)); //set random seed

    /* Get command line arguments */
    if(argc == 4 || argc == 3) {
    
        if(strcmp(argv[2],"FCFS") == 0) {
            if(argc != 3) {
                perror("ERROR: Wrong number of arguments\nUSAGE: ./scheduler [input_file] FCFS\n");
                return -1;
            }
        }
        else if(strcmp(argv[2],"RR") == 0) {
            if(argc != 4) {
                perror("ERROR: Wrong number of arguments\nUSAGE: ./scheduler [input_file] RR [time_quantum]\n");
                return -1;
            }
            
            if(!isNum(argv[3])) {
                perror("ERROR: Time quantum must be a number\n");
                return -1;
            }
            
            time_quantum = atoi(argv[3]);
        }
        else if(strcmp(argv[2],"SRTF") == 0) {
            if(argc != 3) {
                perror("ERROR: Wrong number of arguments\nUSAGE: ./scheduler [input_file] SRTF\n");
                return -1;
            }
        }
        else {
            perror("ERROR: Scheduling type is invalid\n");
            return -1;
        }
        
        in = fopen(argv[1], "r");
        if(in == NULL) {
            perror("ERROR: File not found\n");
            return -1;
        }
    }
    else {
        perror("ERROR: Wrong number of arguments\nUSAGE: ./scheduler [input_file] [FCFS|RR|SRTF] [time_quantum]\n");
        return -1;
    }
    
    /* Initialize queue */
    myqueue.size = 0;
    myqueue.head = NULL;
    myqueue.tail = NULL;
    toArrive.size = 0;
    toArrive.head = NULL;
    toArrive.tail = NULL;
    
    /* Read input file */
    while(1) {
        //tokenize buffer based on spaces
        //put into toArrive queue sorted on arrival time
    }

}

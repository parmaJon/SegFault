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
    Queue ready;
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
    
    /* Initialize queues */
    ready.size = 0;
    ready.head = NULL;
    ready.tail = NULL;
    toArrive.size = 0;
    toArrive.head = NULL;
    toArrive.tail = NULL;
   

    //Example of modified queue use 
   /* Process new = malloc(sizeof(struct process));
    new -> pid = 0;
    enqueue(new,&ready);
    listQueue(&ready);

    new = malloc(sizeof(struct process));
    new -> pid = 1;
    enqueue(new, &toArrive);
    new = malloc(sizeof(struct process));
    new -> pid = 2;
    enqueue(new, &toArrive);
    listQueue(&toArrive);
    clear(&toArrive);
    listQueue(&toArrive);
    
    listQueue(&ready);
    clear(&ready);
    listQueue(&ready);*/


    /* Read input file */
    while(1) {
        break;
        //tokenize buffer based on spaces
        //put into toArrive queue sorted on arrival time
    }

    Process running = NULL;
    int timeStamp = 0;
    
    running = dequeue(toArrive);
    printf("<system time   %d> process %d arrives\n", running->arrival_time, running->pid);
    timeStamp = running->arrival_time;
    int tq = time_quantum;
    int unusedCpuCount = 0;
    int totalTimeWaiting = 0;
    int totalResonseTime = 0;
    int totalTurnaound = 0;
    
    while(toArrive.size > 0 && running != NULL)
    {
        //if new process arrived
        //do not incrament time
        //TODO add prints
        if(timeStamp == toArrive->head->p->arrival_time)
        {
            //if fcfs
                running = FCFS(running, dequeue(toArrive), running->burst_time, ready);
            //if rr
            {
                //check if time quantum has run out and reset it. function call will swap out process
                if(tq == 0)
                {
                    tq = time_quantum;
                    running = roundRobin(running, dequeue(toArrive), 0, ready);
                }   
                else if(running->burst_time == 0)
                {
                    tq = time_quantum;
                    running = roundRobin(running, dequeue(toArrive), 0, ready);
                }
                else
                    running = roundRobin(running, dequeue(toArrive), running->burst_time , ready);
            }
            //if sjtr
                //running = FCFS(running, dequeue(toArrive), running->burst_time, myqueue);
                
            //if the process is just getting responded to    
            if(running->response == false)
            {
                running->response = true;
                totalResponseTime = totalResponseTime + (timeStamp - running->arrival_time);
            }    
        }
        //if process has run its course
        else if(running->burst_time == 0 || tq == 0)
        {
            //if fcfs
                running = FCFS(running, NULL, running->burst_time, ready);
            //if rr
            {
                tq = time_quantum;   
                running = roundRobin(running, NULL, 0, ready);
            }
            //if sjtr
                //running = FCFS(running, NULL, running->burst_time, myqueue);
                
            //if the process is just getting responded to    
            if(running->response == false)
            {
                running->response = true;
                totalResponseTime = totalResponseTime + (timeStamp - running->arrival_time);
            }      
        }
        //else we have finished this timestamp, incrament
        else
        {
            //if rr
                //tq--;
            running->burst_time--;
            timeStamp++;
            totalTurnaround = totalTurnaround + 1 + ready.size;
        }    
        
    }
    
    
    
    
    return 0;



}

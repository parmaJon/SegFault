/******************************************************
 * Filename: myqueue.h
 *
 * Description: This file is the header for myqueue.c
 * and contains the structures used to simulate a
 * process queue
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #2
 * Assigned: Feb. 5, 2016
 * Due: Feb. 19, 2016
******************************************************/

#ifndef _MYQUEUE_H
#define _MYQUEUE_H

#define MAX_PROCESSES 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum {FALSE = 0, TRUE}  bool;
//#define TRUE 1;
//#define FALSE 0;

typedef struct process{
    int pid;
    int arrival_time;
    int burst_time;
} *Process;

typedef struct node{
    struct node *prev;
    struct node *next;
    Process p;
} *Node;

typedef struct {
    int size;
    Node head;
    Node tail;
} Queue;

bool enqueue(Process p, Queue *q);
Process dequeue(Queue *q);
Process target(int pid, Queue *q);
Process randomTarget(Queue *q);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
void clear(Queue *q);
void listQueue(Queue *q);

#endif /* MYQUEUE_H */

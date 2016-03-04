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

#define NUM_REGS 2
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
    int psw;
    int page_table;
    int regs[NUM_REGS];
} *Process;

typedef struct node{
    struct node *prev;
    struct node *next;
    int i;
} *Node;

typedef struct {
    int size;
    Node head;
    Node tail;
} Queue;

Queue myqueue;

bool enqueue(int i);
int dequeue();
int target(int i);
int randomTarget();
bool isEmpty();
bool isFull();
void clear();
void listQueue();

#endif /* MYQUEUE_H */

/******************************************************
 * Filename: myqueue.c
 *
 * Description: This program simulates a process queue
 * utilizing structures defined in myqueue.h and
 * functions implementing the queue operations
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #2
 * Assigned: Feb. 5, 2016
 * Due: Feb. 19, 2016
******************************************************/

#include "myqueue.h"

/**
 * Inserts a process into the end of the queue
 * @param process the process being inserted
 * @return true if successful, false otherwise
 */
bool enqueue(Process process) {
    return false;
}

/**
 * Removes a process from the front of the queue
 * @return the process removed, or NULL in error
 */
Process dequeue() {
    return NULL;
}

/**
 * Removes the process(es) from the queue with the
 * given pid
 * @param pid the pid of the process(es) to be deleted
 */
void delete(int pid) {
    return;
}

/**
 * Determines whether or not the queue is empty or not
 * @return true if the queue is empty, false otherwise
 */
bool isEmpty() {
    return false;
}


bool isFull();
void clear();
void listQueue();

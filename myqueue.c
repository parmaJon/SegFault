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
//To aviod changes to method signatures, need globals
Node *head, *tail;
int length; //for convenience

int main(int argc, char argv[]) {
    return 0;
}

/**
 * Inserts a process into the end of the queue
 * @param process the process being inserted
 * @return true if successful, false otherwise
 */
bool enqueue(Process process) {
    if( isFull() )
        return FALSE;
    Node *new = malloc(sizeof(Node));
    new->next = NULL;
    new->prev = tail;
    new->p = &process;
    tail->next = new;
    tail = new;
    length++;
}

/**
 * Removes a process from the front of the queue
 * @return the process removed, or NULL in error
 */
Process dequeue() {
    Process p;
    length--;
    return p;
}

/**
 * Removes the process(es) from the queue with the
 * given pid
 * @param pid the pid of the process(es) to be deleted
 */
void delete(int pid) {
    length--;
    return;
}

/**
 * Determines whether or not the queue is empty
 * @return true if the queue is empty, false otherwise
 */
bool isEmpty() {
    if( head == NULL && length == 0 )
        return TRUE;
    if( head == NULL || length == 0 )
        printf("QUEUE: ERROR: Head state and queue length inconsistent!");
    return FALSE;
}

/**
 * Determines whether or not the queue is full
 * @return true if the queue is full, false otherwise
 */
bool isFull() {
    if( length < MAX_PROCESSES )
        return FALSE;
    return TRUE;
}

/**
 * Removes all items from the queue
 */
void clear() {
    Node *trav = head;
    Node *prev;

    while( trav != NULL ) {
        free(trav->p);
        prev = trav;
        trav = trav->next;
        free(prev);
    }

    head = NULL;
    tail = NULL;
    length = 0;
}

/**
 * Displays the processes in the queue
 */
void listQueue() {
    return;
}

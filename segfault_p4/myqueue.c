/******************************************************
 * Filename: myqueue.c
 *
 * Description: This program simulates a process queue
 * utilizing structures defined in myqueue.h and
 * functions implementing the queue operations
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

//#define DEBUG
#include "myqueue.h"


/**
 * Inserts a process into the end of the queue
 * @param process the process being inserted
 * @return true if successful, false otherwise
 */
bool enqueue(Process p, Queue *q) {

    if( isFull(q) ) {
        printf("Enqueue error - queue is full\n");
        return FALSE;
    }

    Node new = malloc(sizeof(struct node));
    new -> next = NULL;
    new -> prev = NULL;
    new -> p = p;

    /* Edge case for empty queue */
    if( isEmpty(q) ) {
        q->head = new;
        q->tail = new;
    }
    /* Otherwise, just insert at end */
    else {
        q->tail->next = new;
        new->prev = q->tail;
        q->tail = new;
    }

    q->size++;
    return TRUE;
}

/**
 * Removes a process from the front of the queue
 * @return the process removed, or NULL in error
 */
Process dequeue(Queue *q) {
    if( isEmpty(q) ) {
        return NULL;
    }

    Process p = q->head->p;

    /* Edge case for single element queue */
    if( q->head->next == NULL ) {
        free(q->head);
        q->head = NULL;
        q->tail = NULL;
    }
    else {
        q->head = q->head->next;
        free(q->head->prev);
        q->head->prev = NULL;
    }

    q->size--;
    return p;
}

/**
 * Removes the process(es) from the queue with the
 * given pid
 * @param pid the pid of the process(es) to be deleted
 */
Process target(int pid, Queue *q) {
    Node trav = q->head;
    Node prev;
    Process ret;

    while(trav != NULL) 
    {
 	    //target found
	    if(trav->p->pid == pid){
	        //target at head
	        if(trav -> prev == NULL)
	        {
                    if ( trav -> next )
	                trav -> next -> prev = NULL;
	    	    q->head = trav -> next;
	    	    //if head was tail set both to null
	    	    if(q->head == NULL)
	    	    	q->tail = NULL;
	    	    ret = trav -> p;
	    	    free(trav);
	    	    q->size--;
	    	    return ret;
            }
	        //target at tail
	        else if(trav -> next == NULL)
	        {
    		    q->tail = prev;
	    	    prev -> next = NULL;
	    	    ret = trav -> p;
	    	    free(trav);
	    	    q->size--;
	    	    return ret;
	        }
	        else
	        {
		        prev -> next = trav -> next;
        		trav -> next -> prev = prev;
		        ret = trav -> p;
		        free(trav);
		        q->size--;
		        return ret;
	        }
	    }
	    prev = trav;
	    trav = trav -> next;
    }

    //printf("could not find target\n");
    return NULL;
}

/**
 * Determines whether or not the queue is empty
 * @return true if the queue is empty, false otherwise
 */
bool isEmpty(Queue *q) {
    if( q->head == NULL && q->size == 0 )
        return TRUE;
    if( q->head == NULL || q->size == 0 )
        printf("QUEUE: ERROR: Head state and queue length inconsistent!");
    return FALSE;
}

/**
 * Determines whether or not the queue is full
 * @return true if the queue is full, false otherwise
 */
bool isFull(Queue *q) {
    if( q->size < MAX_PROCESSES )
        return FALSE;
    return TRUE;
}

/**
 * Removes all items from the queue
 */
void clear(Queue *q) {
    Node trav = q->head;
    Node prev;

    while( trav != NULL ) {
      prev = trav;
      trav = trav -> next;
      free(prev->p);
      free(prev);

    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

/**
 * Displays the processes in the queue
 */
void listQueue(Queue *q) {
    Node trav = q->head;
    
    if(trav == NULL){
        printf("queue is empty.\n");
        return;
    }
    printf("[ ");
    while(trav -> next != NULL){
        printf("%d, ", trav -> p -> pid);
        trav = trav -> next;
    }
    printf("%d ]\n", trav -> p -> pid);
    return;
}

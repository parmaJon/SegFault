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
bool enqueue(Process p) {

    if( isFull() ) {
        printf("Enqueue error - queue is full\n");
        return FALSE;
    }

    Node new = malloc(sizeof(struct node));
    new->next = NULL;
    new->prev = myqueue.tail;
    new->p = p;

    /* Edge case for empty queue */
    if( isEmpty() ) {
        myqueue.head = new;
        myqueue.tail = new;
    }
    /* Otherwise, just insert at end */
    else {
        myqueue.tail->next = new;
        myqueue.tail = new;
    }

    myqueue.size++;
    return TRUE;
}

/**
 * Removes a process from the front of the queue
 * @return the process removed, or NULL in error
 */
Process dequeue() {
    if( isEmpty() ) {
        return NULL;
    }

    Process p = myqueue.head->p;

    /* Edge case for single element queue */
    if( myqueue.head->next == NULL ) {
        free(myqueue.head);
        myqueue.head = NULL;
        myqueue.tail = NULL;
    }
    else {
        myqueue.head = myqueue.head->next;
        free(myqueue.head->prev);
        myqueue.head->prev = NULL;
    }

    myqueue.size--;
    return p;
}

/**
 * Removes the process(es) from the queue with the
 * given pid
 * @param pid the pid of the process(es) to be deleted
 */
Process target(int pid) {
    Node trav = myqueue.head;
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
	    	    myqueue.head = trav -> next;
	    	    //if head was tail set both to null
	    	    if(myqueue.head == NULL)
	    	    	myqueue.tail = NULL;
	    	    ret = trav -> p;
	    	    free(trav);
	    	    myqueue.size--;
	    	    return ret;
            }
	        //target at tail
	        else if(trav -> next == NULL)
	        {
    		    myqueue.tail = prev;
	    	    prev -> next = NULL;
	    	    ret = trav -> p;
	    	    free(trav);
	    	    myqueue.size--;
	    	    return ret;
	        }
	        else
	        {
		        prev -> next = trav -> next;
        		trav -> next -> prev = prev;
		        ret = trav -> p;
		        free(trav);
		        myqueue.size--;
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
bool isEmpty() {
    if( myqueue.head == NULL && myqueue.size == 0 )
        return TRUE;
    if( myqueue.head == NULL || myqueue.size == 0 )
        printf("QUEUE: ERROR: Head state and queue length inconsistent!");
    return FALSE;
}

/**
 * Determines whether or not the queue is full
 * @return true if the queue is full, false otherwise
 */
bool isFull() {
    if( myqueue.size < MAX_PROCESSES )
        return FALSE;
    return TRUE;
}

/**
 * Removes all items from the queue
 */
void clear() {
    Node trav = myqueue.head;
    Node prev;

    while( trav != NULL ) {
      prev = trav;
      trav = trav -> next;
      free(prev->p);
      free(prev);

    }

    myqueue.head = NULL;
    myqueue.tail = NULL;
    myqueue.size = 0;
}

/**
 * Displays the processes in the queue
 */
void listQueue() {
    Node trav = myqueue.head;
    
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

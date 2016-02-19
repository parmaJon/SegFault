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
//Yes, but wrong global and wrong place, see .h file queue
Node *head, *tail;
int length; //for convenience

int main(int argc, char argv[]) {

    int i = 0;
    bool retFlag;
    char *command;
    char *input;
    Process newProcess;
    char *regStr;
    while(1) {

        scanf("%[^\n]%*c",&input);
        if(strcmp((command = strtok(input, " ")),"enqueue") == 0) {

            printf("Enqueue Reached\n");
            newProcess.pid = atoi(strtok(NULL, " "));
            newProcess.psw = atoi(strtok(NULL, " "));
            newProcess.page_table = atoi(strtok(NULL, " "));
            for(i = 0, regStr = strtok(NULL, " "); regStr != NULL; i++, regStr = strtok(NULL, " ")) {

                newProcess.regs[i] = atoi(regStr);
            }

            retFlag = enqueue(newProcess);

            if(retFlag == FALSE) {
                printf("Enqueue failed, queue is full\n");
            }
        }

        else if(strcmp(command,"dequeue") == 0) {

            printf("Dequeue Reached\n");
            newProcess = dequeue();
            printf("PID: %i\nPSW: %i\nPage Table: %i\nRegs:",newProcess.pid,newProcess.psw,newProcess.page_table);
            for(i = 0; i < NUM_REGS; i++) {
                printf(" %i",newProcess.regs[i]);
            }
            printf("\n");
        }

        else if(strcmp(command,"delete") == 0) {

            printf("Delete Reached\n");
            i = atoi(strtok(NULL, " "));

            delete(i);
        }

        else if(strcmp(command,"list") == 0) {

            printf("List Reached\n");
            listQueue();
        }

        else if(strcmp(command,"quit") == 0) {
            break;
        }

        else {
            printf("Invalid Entry\n");
        }
    }

    return FALSE;
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

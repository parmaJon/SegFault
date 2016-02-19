/******************************************************
 * Filename: myqueue.c
 *
 * Description: This program simulates a process queue
 * utilizing structures defined in myqueue.h and
 * functions implementing the queue operations
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant, Garrett Ransom, Jonathan Brandt, Travis Nordquist
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #2
 * Assigned: Feb. 5, 2016
 * Due: Feb. 19, 2016
******************************************************/

#include "myqueue.h"

int main(int argc, char argv[]) {

    int i = 0;
    bool retFlag;
    char *command;
    char *input;
    char buffer[100];
    Process newProcess;
    Process retProcess;
    char *regStr;

    while(1) {
        printf("Enter a queue operation:\n > enqueue [pid] [psw] [page_table] [reg0] [reg1]...\n > dequeue\n > delete [pid]\n > list\n > quit\n > ");
        scanf("%[^\n]%*c", buffer);
        input = malloc(strlen(buffer) * sizeof(char));
        strncpy(input, buffer, strlen(buffer));

        command = strtok(input, " ");

        if(strcmp(command,"enqueue") == 0) {

            newProcess = malloc(sizeof(Process));
            newProcess->pid = atoi(strtok(NULL, " "));
            newProcess->psw = atoi(strtok(NULL, " "));
            newProcess->page_table = atoi(strtok(NULL, " "));
            for(i = 0, regStr = strtok(NULL, " "); regStr != NULL; i++, regStr = strtok(NULL, " ")) {

                newProcess->regs[i] = atoi(regStr);
            }

            retFlag = enqueue(newProcess);

            if(retFlag == FALSE) {
                perror("Enqueue failed, queue is full\n");
            }
        }

        else if(strcmp(command,"dequeue") == 0) {

            retProcess = dequeue();

            if(retProcess == NULL) {
                perror("Dequeue Failed: Queue is empty\n");
            }

            else {
                printf("PID: %i\nPSW: %i\nPage Table: %i\nRegs:",retProcess->pid,retProcess->psw,retProcess->page_table);
                for(i = 0; i < NUM_REGS; i++) {
                    printf(" %i",retProcess->regs[i]);
                }
                printf("\n");
            }
        }

        else if(strcmp(command,"delete") == 0) {

            i = atoi(strtok(NULL, " "));

            delete(i);
        }

        else if(strcmp(command,"list") == 0) {

            listQueue();
        }

        else if(strcmp(command,"quit") == 0) {
            break;
        }

        else {
            printf("Invalid Entry\n");
        }

        free(input);
    }

    return FALSE;
}

/**
 * Inserts a process into the end of the queue
 * @param process the process being inserted
 * @return true if successful, false otherwise
 */
bool enqueue(Process process) {

    if( isFull() ) {
        return FALSE;
    }

    Node new = malloc(sizeof(Node));
    new->next = NULL;
    new->prev = myqueue.tail;
    new->p = process;

    if( isEmpty() ) {
        myqueue.head = new;
        myqueue.tail = new;
    }

    else {
        myqueue.tail->next = new;
        myqueue.tail = new;
    }

    myqueue.size++;
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
    myqueue.head = myqueue.head->next;
    myqueue.size--;
    return p;
}

/**
 * Removes the process(es) from the queue with the
 * given pid
 * @param pid the pid of the process(es) to be deleted
 */
void delete(int pid) {
    Node prev, cur, next;

    if( isEmpty() ) {
        perror("Delete Failed: Queue is empty\n");
    }

    if(myqueue.size == 1) {
        if(myqueue.head->p->pid == pid) {
            free(myqueue.head);
            myqueue.head = NULL;
            myqueue.tail = NULL;
            myqueue.size--;
        }

        else {
            perror("Delete Failed: PID not found\n");
        }
        return;
    }

    else if(myqueue.size == 2) {

        if(myqueue.head->p->pid == pid) {
            free(myqueue.head);
            myqueue.head = myqueue.tail;
            myqueue.head->prev = NULL;
            myqueue.size--;
        }

        else if(myqueue.tail->p->pid == pid) {
            free(myqueue.tail);
            myqueue.tail = myqueue.head;
            myqueue.head->next = NULL;
            myqueue.size--;
        }

        else {
            perror("Delete Failed: PID not found\n");
        }
        return;
    }

    else {

        prev = myqueue.head;
        cur = prev->next;
        next = cur->next;

        if(prev->p->pid == pid) {
            free(prev);
            myqueue.head = cur;
            cur->prev = NULL;
            myqueue.size--;
        }

        for(; next != NULL; prev = cur, cur = next, next = cur->next) {

            if(cur->p->pid == pid) {
                free(cur);
                prev->next = next;
                next->prev = prev;
                myqueue.size--;
            }
        }

        if(cur->p->pid == pid) {
            free(cur);
            prev->next = next;
            myqueue.size--;
        }

        return;
    }
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
        free(trav->p);
        prev = trav;
        trav = trav->next;
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
    return;
}

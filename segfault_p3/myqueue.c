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

//#define DEBUG
#include "myqueue.h"

int main(int argc, char argv[]) {

    int i = 0;
    bool retFlag;
    char *command;
    char *input;
    char buffer[100] = {0};
    Process newProcess;
    Process retProcess;
    char *regStr;
    char *tmp;

    /* Loop to perform commands */
    while(1) {
        printf("\nEnter a queue operation:\n  - enqueue [pid] [psw] [page_table] [reg0] [reg1]...\n  - dequeue\n  - delete [pid]\n  - list\n  - quit\n > ");
        //scanf("%[^\n]%*c", buffer);
        /* Read user input into the buffer */
        for (i = 0; i < 100; i++) { 
            buffer[i] = fgetc(stdin);
            if( buffer[i] == '\n' ) {
                buffer[i] = 0;
                break;
            }
        }
        if( buffer[99] != 0 ) //if input too long, append null
            buffer[99] = 0;

        #ifdef DEBUG
        printf("DEBUG: Interface recieved = %s\n", buffer);
        #endif


        input = malloc((strlen(buffer)+1) * sizeof(char));
        memcpy(input, buffer, strlen(buffer)+1);

        #ifdef DEBUG
        printf("DEBUG: Copied command string = %s\n", input);
        #endif

        /* Perform analysis on copy of the input */
        command = strtok(input, " ");

        if(strcmp(command,"enqueue") == 0) {

            tmp = strtok(NULL, " ");
            char *psw = strtok(NULL, " ");
            char *pt = strtok(NULL, " ");
            #ifdef DEBUG
            printf("DEBUG: Interface commanded to enqueue pid = %s\n  psw = %s\n  pt = %s\n",tmp,psw,pt);
            #endif
 
            /* Ensure that all args for enqueue were properly entered */
            if( !tmp  ||  !psw  ||  !pt ) {
                errno = EINVAL;
                perror("Error: Specify pid, psw, and page_table (at minimum) for enqueue");
                retFlag = FALSE;
            }
            else { //create and enqueue the new process
                newProcess = malloc(sizeof(struct process));
                newProcess->pid = atoi(tmp);
                newProcess->psw = atoi(psw);
                newProcess->page_table = atoi(pt);
                for(i = 0, regStr = strtok(NULL, " "); i < NUM_REGS; i++, regStr = strtok(NULL, " ")) {
                    #ifdef DEBUG
                    printf("  reg%d = %s\n",i,regStr);
                    #endif
                    if( regStr != NULL ) {
                        newProcess->regs[i] = atoi(regStr);
                    }
                    else { //register values default to zero
                        newProcess->regs[i] = 0;
                    }
                }
                retFlag = enqueue(newProcess);
            }

            /* Identify return status to user, just in case */
            if(retFlag == FALSE) {
                perror("Enqueue failed");
            }
            else {
                printf("Process queued\n");
            }
        }

        else if(strcmp(command,"dequeue") == 0) {

            retProcess = dequeue();

            /* If dequeue failed, queue was empty */
            if(retProcess == NULL) {
                errno = EPERM;
                perror("Dequeue Failed: Queue is empty");
            }
            /* Otherwise, print process elements */
            else {
                printf("\nPID: %i\nPSW: %i\nPage Table: %i\nRegs:",retProcess->pid,retProcess->psw,retProcess->page_table);
                for(i = 0; i < NUM_REGS; i++) {
                    printf(" %i",retProcess->regs[i]);
                }
                printf("\n");

                free(retProcess);
            }
        }

        else if(strcmp(command,"delete") == 0) {
            tmp = strtok(NULL, " ");
            #ifdef DEBUG
            printf("DEBUG: Interface commanded to delete pid = %s\n",tmp);
            #endif

            /* Check that pid was specified */
            if( !tmp ) {
                errno = EINVAL;
                perror("Error: Specify pid to delete");
            }
            else {  //if so, execute the delete op
                i = atoi(tmp);
                delete(i);
            }
        }

        else if(strcmp(command,"list") == 0) {
            #ifdef DEBUG
            printf("DEBUG: Interface commanded to list queue contents\n");
            #endif
            listQueue();
        }

        else if(strcmp(command,"quit") == 0) {
            #ifdef DEBUG
            printf("DEBUG: Interface commanded to quit\n");
            #endif

            /* Before quitting, free memeory */
            clear();
            free(input);
            break;
        }

        else { //if command not recognized
            printf("Invalid Entry\n");
        }

        errno = 0;
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
        printf("Enqueue error - queue is full");
        return FALSE;
    }

    Node new = malloc(sizeof(struct node));
    new->next = NULL;
    new->prev = myqueue.tail;
    new->p = process;

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
void delete(int pid) {
    Node prev, cur, next;
    Process proc;
    int i;

    #ifdef DEBUG
    printf("DEBUG: Deleting pid = %d\n",pid);
    #endif

    if( isEmpty() ) {
        errno = EPERM;
        perror("Delete Failed: Queue is empty");
        return;
    }

    /* Edge case for single element queue */
    if(myqueue.size == 1) {
        if(myqueue.head->p->pid == pid) {
            proc = dequeue();
            printf("\nPID: %i\nPSW: %i\nPage Table: %i\nRegs:",proc->pid,proc->psw,proc->page_table);
            for(i = 0; i < NUM_REGS; i++) {
                printf(" %i",proc->regs[i]);
            }
            printf("\n");
            free(proc);
        }
        else {
            errno = EINVAL;
            perror("Delete Failed: PID not found");
        }
        return;
    }
    else {

        prev = NULL;
        cur = myqueue.head;
        next = cur->next;

        /* Loop through list, deleting any matching processes */
        for(; next != NULL; prev = cur, cur = next, next = cur->next) {

            if(cur->p->pid == pid) {
                /* Special case for head of queue */
                if( myqueue.head == cur )
                    myqueue.head = next;

                proc = cur->p;
                printf("\nPID: %i\nPSW: %i\nPage Table: %i\nRegs:",proc->pid,proc->psw,proc->page_table);
                for(i = 0; i < NUM_REGS; i++) {
                    printf(" %i",proc->regs[i]);
                }
                printf("\n");
                free(proc);
                free(cur);

                if( prev ) //check that prev is not NULL
                    prev->next = next;

                next->prev = prev;
                myqueue.size--;
                cur = prev; //set cur to prev so that prev will remain unchanged in next iteration
            }
        }

        /* Tail element is a match, delete that as well */
        if( cur != NULL  &&  cur->p->pid == pid) {
            /* Special case for head of queue */
            if( myqueue.head == cur )
                myqueue.head = next;

            proc = cur->p;
            printf("\nPID: %i\nPSW: %i\nPage Table: %i\nRegs:",proc->pid,proc->psw,proc->page_table);
            for(i = 0; i < NUM_REGS; i++) {
                printf(" %i",proc->regs[i]);
            }
            printf("\n");
            myqueue.tail = prev;
            free(proc);
            free(cur);

            if( prev )
                prev->next = NULL;
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
    Node trav = myqueue.head;
    
    if(trav == NULL){
        printf("queue is empty.\n");
        return;
    }
    while(trav -> next != NULL){
        printf("%d, ", trav -> p -> pid);
        trav = trav -> next;
    }
    printf("%d.\n", trav -> p -> pid);
    return;
}

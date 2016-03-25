/******************************************************
 * Filename: functions.c
 *
 * Description: contains the scheduling algorithms
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
#include "functions.h"

/**
 * Implements the scheduling logic for Round Robin.  This will place
 * jobs into the ready queue, preempt a running job, and set a new running 
 * job as necessary.
 * @param Process running, the currently executing process
 * @param Process new, a newely arrived process
 * @param int timeRemaining, time left in execution of the running process
 * @param Queue ready, the process queue
 * @return The scheduled running process
 */
Process roundRobin(Process running, Process new, int timeRemaining, Queue *ready)
{
    //if we need to cycle running to end of queue
    if(timeRemaining < 1)
    {
        //if we did not get a new process
        if(new == NULL)
        {
            if(running == NULL)
            {
                return NULL;
            }
            if(running->burst_time > 0)
            {
                enqueue(running, ready);
                return dequeue(ready);
            }
            
            else if(ready->size > 0)
            {
                printf(" process %d finished\n", running->pid);
                free(running);
                return dequeue(ready);
            }    
            else
            {
                printf(" process %d finished\n", running->pid);
                free(running);
                return NULL;
            }
        }
        //if we got a new process
        else
        {
            if(running == NULL)
            {
                return new;
            }
            
            enqueue(new, ready);
            if(running->burst_time > 0)
            {
                enqueue(running, ready);
                return dequeue(ready);
            }
            
            else
            {
                printf(" process %d finished\n", running->pid);
                free(running);
                return dequeue(ready);
            }    
        }
        
    }
    //if process is not finished
    else
    {
        //if we did not get a new process
        if(new == NULL){}
            //dont think anything is neccesary just a check
        
        //if we got a new process
        else
            enqueue(new, ready);
        
        return running;
    }
}


/**
 * This process implements firstcome-first served for the queue of processes, queues a new one
 * if needed, dequeues the running process if burst time has elapsed
 * @param Process running, the currently executing process
 * @param Process new, a newely arrived process
 * @param int timeRemaining, time left in execution of running
 * @return the finished process if finished, else NULL if process needs more time
 */
Process fcfs(Process running, Process new, int timeRemaining, Queue *queue)
{
/*  //if no new process recieved
  if(new == NULL){
	
	//if current running process will finish
	if(timeRemaining < 1)
	{
	    if(running == NULL)
        {
            return NULL;
        }
        printf(" process %d finished\n", running->pid);
	    free(running);
  	    return dequeue(queue);
	}
	else
	  return running;
  }
   //new process recieved
  else{*/
  
    if(running == NULL)
        return new;
    
    if(new != NULL)
    	enqueue(new, queue);

    if(timeRemaining < 1) {
        printf(" process %d finished\n", running->pid);
        free(running);
	    return dequeue(queue);
    }
    else
        return running;

}


/**
 * Implements the scheduling logic for Shortest Remaining Time First.  This will 
 * place jobs into the ready queue, preempt a running job, and set a new running 
 * job as necessary.
 * @param Process running, the currently executing process
 * @param Process new, a newely arrived process
 * @param int timeRemaining, time left in execution of the running process
 * @param Queue ready, the process queue
 * @return The scheduled running process
 */
Process srtf(Process running, Process new, int timeRemaining, Queue *ready)
{
  //if no new processes
  if(new == NULL) {

    //if current process will finish, clear it and start next in queue
    if(timeRemaining < 1) {
      if(running == NULL)
        return NULL;
      
      printf(" process %d finished\n", running->pid);
      free(running);
      return dequeue(ready);
    }
    else {
      return running; //should never be called like this
    }

  }
  else {

    //if current process will finish, clear it
    if(timeRemaining < 1) {
      if(running == NULL)
      {
        return new;
      }
      printf(" process %d finished\n", running->pid);
      free(running);
      running = NULL;
    }

    //if the new process has a shorter remaining time, replace the running proc
    if(timeRemaining > new->burst_time) {
      if (running)
        enqueueSRTF(running, ready);
      return new;
    }
    else {  //otherwise, put the new proc in the ready queue
      enqueueSRTF(new, ready);
    }

    //if nothing is currently running, pull from ready queue
    if(!running)
      return dequeue(ready);

  }

  return running;
}



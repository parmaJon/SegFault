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

/**
 * recieves a message from the running state and handles it
 * @param Process running, the currently executing process
 * @param Process new, a newely arrived process
 * @param int timeRemaining, time left in execution of running
 * @param Queue queue, the process queue
 * @return the process removed, or NULL in error
 */
Process roundRobin(Process running, Process new, int timeRemaining, Queue *queue)
{
    //if we need to cycle running to end of queue
    if(timeRemaining < 1)
    {
        //if we did not get a new process
        if(new == NULL)
        {
            if(enqueue != NULL)   //enqueue? is this meant to be queue?
                enqueue(running);
            if(queue.size > 0)
                return dequeue();
            else
                //TODO Return empty statement
                return ;
        }
        //if we got a new process
        else
        {
            enqueue(new);
            if(enqueue != NULL)  //same here, is enqueue meant to be queue
                enqueue(running);
            if(queue.size > 0)  //because the new proc is queued can skip the check and just dequeue?
                return dequeue();
            else
                //TODO Return empty statement
                return ;
        }
        
    }
    //if process is not finished  <-- should this be if process IS finished?
    else
    {
        //if we did not get a new process
        if(new == NULL)
            //dont think anything is neccesary just a check
        
        //if we got a new process
        else
            enqueue(new);
        
        return running;
    }
}


/**this may be easier to implement as a default of main instead of a new function
*this process implements firstcome-first served for the queue of processes, queues a new one
* if needed, dequeues the running process if last needed cycle
* @param Process running, the currently executing process
 * @param Process new, a newely arrived process
 * @param int timeRemaining, time left in execution of running
 * @return the finished process if finished, else NULL if process needs more time
*/
Process fcfs(Process running, Process new, int timeRemaining)
{
  //if no new process recieved
  if(new == NULL){
	
	//if current running process will finish
	if(timeRemaining < 1)
  	  return dequeue();
	
	else
	  return NULL;
  }
   //new process recieved
  else{
    enqueue(new);

    if(timeRemaining < 1) {
	    return dequeue();
    }
    else {
        return NULL;
    }
  }
}



Process srtf(Process running, Process new, int timeRemaining, Queue *ready)
{
  //if no new processes
  if(new == NULL) {

    //if current process will finish, clear it and start next in queue
    if(timeRemaining < 1) {
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



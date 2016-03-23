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
Process roundRobin(Process running, Process new, int timeRemaining, Queue queue)
{
    //if we need to cycle running to end of queue
    if(timeRemaining < 1)
    {
        //if we did not get a new process
        if(new == NULL)
        {
            if(enqueue != NULL)
                enqueue(running);
            if(myqueue.size > 0)
                return dequeue();
            else
                //TODO Return empty statement
                return ;
        }
        //if we got a new process
        else
        {
            enqueue(new);
            if(enqueue != NULL)
                enqueue(running);
            if(myqueue.size > 0)
                return dequeue();
            else
                //TODO Return empty statement
                return ;
        }
        
    }
    //if process is not finished
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









/******************************************************
 * Filename: myqueue.h
 *
 * Description: This file is the header for myqueue.c
 * and contains the structures used to simulate a
 * process queue
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

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

Process roundRobin(Process running, Process new, int timeRemaining, Queue *queue);
Process fcfs(Process running, Process new, int timeRemaining, Queue *queue);
Process srtf(Process running, Process new, int timeRemaining, Queue *ready);

#endif /* FUNCTIONS.H */

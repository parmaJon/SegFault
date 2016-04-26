/******************************************************
 * Filename: main.c
 *
 * Description: This program simulates scheduling
 * algorithms by queueing and using process information
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant, Garrett Ransom,
 *     Jonathan Brandt, Travis Nordquist
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #6
 * Assigned: Apr. , 2016
 * Due: May 6, 2016
******************************************************/

#include "disk.h"


int main(int argc, char *argv[]) {

	make_fs("test");
	mount_fs("test");
	unmount_fs("test");
    
    return 0;

}

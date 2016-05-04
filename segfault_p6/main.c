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

#include <stdio.h>
#include <stdlib.h>

#include "disk.h"


int main(int argc, char *argv[]) {

	make_fs("test");
	mount_fs("test");

	fs_create("myfile.txt");
	fs_list_files();
	fs_create("junk.txt");
	fs_list_files();

	int fd = fs_open("myfile.txt");
	char *buf = "t";
	fs_write(fd,buf,2);
	fs_close(fd);

	printf("size - %d\n", fs_get_filesize("myfile.txt"));

	unmount_fs("test");

	mount_fs("test");

	fs_list_files();
	printf("size - %d\n", fs_get_filesize("myfile.txt"));
	printf("size - %d\n", fs_get_filesize("junk.txt"));
	fs_delete("myfile.txt");
	fs_list_files();

	unmount_fs("test");
	
	return 0;
	
}

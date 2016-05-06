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
#include <string.h>

#include "disk.h"


int main(int argc, char *argv[]) {

	make_fs("test");
	mount_fs("test");

	fs_create("myfile.txt");
	fs_list_files();
	fs_create("junk.txt");
	fs_list_files();

	int fd = fs_open("myfile.txt");
	char *buf = "testing this code";
	fs_write(fd,buf,strlen(buf));
	//fs_lseek(fd,100);
	buf = "it! Blahc lksjdfkj ihwtw lekjth lksajdf 123413459827 i sadfkjhq3oipu4tryheiq8u934yt5981 3yq4t97ughergo9vyq398 4y5t 98qgrebgfvkje";
	fs_write(fd,buf,strlen(buf)+1);
	//fs_truncate(fd,2);
	//fs_write(fd,"",1);
	fs_close(fd);

	fd = fs_open("myfile.txt");
	char word[1000];
	fs_read(fd,word,10000);
	fs_close(fd);

	printf("\"%s\" was read from myfile.txt\n",word);

	printf("size of myfile.txt - %d\n", fs_get_filesize("myfile.txt"));

	unmount_fs("test");

	mount_fs("test");

	fs_list_files();
	printf("size of myfile.txt - %d\n", fs_get_filesize("myfile.txt"));
	printf("size of junk.txt - %d\n", fs_get_filesize("junk.txt"));

	fd = fs_open("myfile.txt");
	fs_read(fd,word,10000);
	fs_close(fd);
	printf("\"%s\" was read from myfile.txt\n",word);

	fs_delete("myfile.txt");
	fs_list_files();

	unmount_fs("test");
	
	return 0;
	
}

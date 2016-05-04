#include <semaphore.h>

#ifndef _DISK_H_
#define _DISK_H_

//******************************************************************************
#define DISK_BLOCKS  8192      // number of blocks on the disk (half is for overhead)               
#define BLOCK_SIZE   4096      // block size on "disk"                        

//all the structures!!

typedef struct superBlock{
	int master;
	int free1;
	int free2;
	int dir;
	int inodes;
	int data;
	int free;
} *SuperBlock;

typedef struct indexBlockLayer2{
	void* pointers[512];
} *IndexBlockLayer2;

typedef struct indexBlock{
	IndexBlockLayer2 pointers[512];
} *IndexBlock;

typedef struct iNode{
	char name[64];
	int size;
	sem_t write, read;
	IndexBlock data;
} *INode;

typedef struct directory{
	INode inodes[64];
} *Directory;

typedef struct fd{
	int seek; //number of characters offset
	INode pointer;
} *Fd;

typedef struct masterControlBlock{
	Fd descriptors[32];
	IndexBlock free;
	Directory dir;
} *MasterControlBlock;


//******************************************************************************
int make_disk(char *name);     // create an empty, virtual disk file          
int open_disk(char *name);     // open a virtual disk (file)                  
int close_disk();              // close a previously opened disk (file)    
int make_fs(char *disk_name);
int mount_fs(char *disk_name);
int unmount_fs(char *disk_name);
int fs_open(char *name);
int fs_write(int fildes, void *buf, size_t nbyte);
int fs_read(int fildes, void *buf, size_t nbyte);
int fs_get_filesize(char* name);
int fs_create(char *name);
int fs_delete(char *name);
int fs_list_files();
int fs_close(int fd);

int block_write(int block, char *buf);
                               // write a block of size BLOCK_SIZE to disk    
int block_read(int block, char *buf);
                               // read a block of size BLOCK_SIZE from disk   
//******************************************************************************

#endif

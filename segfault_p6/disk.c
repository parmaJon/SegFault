#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "disk.h"

/******************************************************************************/
static int active = 0;  /* is the virtual disk open (active) */
static int handle;      /* file handle to virtual disk       */
static MasterControlBlock master;
static SuperBlock super;

/******************************************************************************/

int make_fs(char *disk_name)
{
	if(make_disk(disk_name) == -1)
		return -1;
		
	if(open_disk(disk_name) == -1)
		return -1;
	
	SuperBlock sup = malloc(sizeof(struct superBlock));
	sup->master = 1;
	sup->free1 = 2;
	sup->free2 = 3;
	sup->dir = 11;
	sup->inodes = 12;
	sup->data = 652;
	sup->free = 652;
	block_write(0,(char *)sup);
	free(sup);
	
	if( close_disk(disk_name) == -1 )
		return -1;
		
	return 0;
}

int mount_fs(char *disk_name)
{
	int k,j=0,bsize,dataOffset=0;
	

	if(open_disk(disk_name) == -1)
		return -1;
	
	//super block
	super = malloc(sizeof(struct superBlock));
	block_read(0,(char *)super);
	printf("%i\n",super->data);
	
	//master control block
	master = malloc(sizeof(struct masterControlBlock));
	block_read(super->master,(char *)master);

	//file descriptors
	int i;
	for(i = 0; i<32; i++)
	{
		master->descriptors[i] = malloc(sizeof(struct fd));
		master->descriptors[i]->seek = 0;
		master->descriptors[i]->pointer = NULL;
	}
	
	//free index block
	IndexBlock free = malloc(sizeof(struct indexBlock));
	block_read(super->free1,(char *)free);
	master->free = free;

	
	//top directory
	master->dir = malloc(sizeof(struct directory));
	block_read(super->dir,(char *)master->dir);
	
	//files
	for(i = 0; i<64; i++)
	{
		dir->inodes[i] = malloc(sizeof(struct iNode));
		block_read(super->inodes + (i*10),(char *)dir->inodes[i]);
		dir->inodes[i]->data = malloc(sizeof(struct indexBlock));
		block_read(super->inodes + 1 + (i*10),(char *)dir->inodes[i]->data);
		INode inode = dir->inodes[i]; //convenience var
		
		//inodes (layer 2)
		for(k = 0; k<8; k++) {
			dir->inodes[i]->data->pointers[k] = malloc(sizeof(struct indexBlockLayer2));
			block_read(super->inodes + 2 + k + (i*10),(char *)dir->inodes[i]->data->pointers[k]);
			IndexBlockLayer2 ib = dir->inodes[i]->data->pointers[k]; //convenience var
			
			//file content
			bsize = (inode->size/BLOCK_SIZE)+1;
			while( j < bsize ) {
				inode->data->pointers[k]->pointers[j] = malloc(BLOCK_SIZE);
				block_read(super->data + dataOffset,(char *)inode->data->pointers[k]->pointers[j]);
				j++;
			}
			dataOffset += bsize;
		} 
	}
	
	k = 0;
		
	//free index blocks (layer 2)
	for(i = 0; i<8; i++)
	{
		free->pointers[i] = malloc(sizeof(struct indexBlockLayer2));
		block_read(super->free2 + i,(char *)free->pointers[i]);
		
		j = 0;
		while( k < BLOCK_SIZE  &&  j < (BLOCK_SIZE/8) ) {
			free->pointers[i]->pointers[k%(BLOCK_SIZE/8)] = malloc(BLOCK_SIZE);
			block_read(super->free + k,(char *)free->pointers[i]->pointers[k%(BLOCK_SIZE/8)]);
			k++;
			j++;
		}
	}
	
	
	return 0;
}

int unmount_fs(char *disk_name)
{
	int freespace = 0;

	//super block
	SuperBlock super = malloc(sizeof(struct superBlock));
	super->master = 1;
	super->free1 = 2;
	super->free2 = 3;
	super->dir = 11;
	super->inodes = 12;
	super->data = 652; //need to determine free offset
	
	//master control block
	block_write(super->master,(char *)master);

	//file descriptors
	int i;
	for(i = 0; i<32; i++)
	{
		free(master->descriptors[i]);
	}
	
	//free index block
	IndexBlock free = master->free;
	block_write(super->free1,(char *)free);
	
	//free index blocks (layer 2)
	for(i = 0; i<8; i++)
	{
		block_write(super->free2 + i,(char *)free->pointers[i]);
	}
	
	//top directory
	master->dir = malloc(sizeof(struct directory));
	block_read(super->dir,(char *)master->dir);
	
	//files
	for(i = 0; i<64; i++)
	{
		dir->inodes[i] = malloc(sizeof(struct iNode));
		block_read(super->inodes + (i*10),(char *)dir->inodes[i]);
		dir->inodes[i]->data = malloc(sizeof(struct indexBlock));
		block_read(super->inodes + 1 + (i*10),(char *)dir->inodes[i]->data);
		INode inode = dir->inodes[i]; //convenience var
		
		//inodes (layer 2)
		for(k = 0; k<8; k++) {
			dir->inodes[i]->data->pointers[k] = malloc(sizeof(struct indexBlockLayer2));
			block_read(super->inodes + 2 + k + (i*10),(char *)dir->inodes[i]->data->pointers[k]);
			IndexBlockLayer2 ib = dir->inodes[i]->data->pointers[k]; //convenience var
			
			//file content
			bsize = (inode->size/BLOCK_SIZE)+1;
			while( j < bsize ) {
				inode->data->pointers[k]->pointers[j] = malloc(BLOCK_SIZE);
				block_read(super->data + dataOffset,(char *)inode->data->pointers[k]->pointers[j]);
			}
			dataOffset += bsize;
		} 
	}
	
	
	free(master->dir);
	
	//free layer 2 free index blocks
	for(i = 0; i<8; i++)
	{
		free(free->pointers[i]);
	}
	
	free(free);
	free(master);
	
	block_write(0,(char *)super);
	free(super);

	if( close_disk(disk_name) == -1 )
		return -1;

	return 0;
}	














int make_disk(char *name)
{ 
  int f, cnt;
  char buf[BLOCK_SIZE];

  if (!name) {
    fprintf(stderr, "make_disk: invalid file name\n");
    return -1;
  }

  if ((f = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
    perror("make_disk: cannot open file");
    return -1;
  }

  memset(buf, 0, BLOCK_SIZE);
  for (cnt = 0; cnt < DISK_BLOCKS; ++cnt)
    write(f, buf, BLOCK_SIZE);

  close(f);

  return 0;
}

int open_disk(char *name)
{
  int f;

  if (!name) {
    fprintf(stderr, "open_disk: invalid file name\n");
    return -1;
  }  
  
  if (active) {
    fprintf(stderr, "open_disk: disk is already open\n");
    return -1;
  }
  
  if ((f = open(name, O_RDWR, 0644)) < 0) {
    perror("open_disk: cannot open file");
    return -1;
  }

  handle = f;
  active = 1;

  return 0;
}

int close_disk()
{
  if (!active) {
    fprintf(stderr, "close_disk: no open disk\n");
    return -1;
  }
  
  close(handle);

  active = handle = 0;

  return 0;
}

int block_write(int block, char *buf)
{
  if (!active) {
    fprintf(stderr, "block_write: disk not active\n");
    return -1;
  }

  if ((block < 0) || (block >= DISK_BLOCKS)) {
    fprintf(stderr, "block_write: block index out of bounds\n");
    return -1;
  }

  if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
    perror("block_write: failed to lseek");
    return -1;
  }

  if (write(handle, buf, BLOCK_SIZE) < 0) {
    perror("block_write: failed to write");
    return -1;
  }

  return 0;
}

int block_read(int block, char *buf)
{
  if (!active) {
    fprintf(stderr, "block_read: disk not active\n");
    return -1;
  }

  if ((block < 0) || (block >= DISK_BLOCKS)) {
    fprintf(stderr, "block_read: block index out of bounds\n");
    return -1;
  }

  if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
    perror("block_read: failed to lseek");
    return -1;
  }

  if (read(handle, buf, BLOCK_SIZE) < 0) {
    perror("block_read: failed to read");
    return -1;
  }

  return 0;
}

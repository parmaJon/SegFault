#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "disk.h"

//#define DEBUG

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
	
	SuperBlock sup = calloc(1,BLOCK_SIZE);
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
	if(active) {
		errno = 1;
		perror("A disk is already mounted!");
	}

	int k,j=0,bsize,dataOffset=0;
	

	if(open_disk(disk_name) == -1)
		return -1;
	
	//super block
	super = malloc(BLOCK_SIZE);
	block_read(0,(char *)super);
	
	//master control block
	master = calloc(1,BLOCK_SIZE);
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
	master->dir = calloc(1,BLOCK_SIZE);
	block_read(super->dir,(char *)master->dir);
  Directory dir = master->dir;
	
	//files
	for(i = 0; i<64; i++)
	{
		dir->inodes[i] = calloc(1,BLOCK_SIZE);
		block_read(super->inodes + (i*10),(char *)dir->inodes[i]);
		dir->inodes[i]->data = calloc(1,BLOCK_SIZE);
		block_read(super->inodes + 1 + (i*10),(char *)dir->inodes[i]->data);
		INode inode = dir->inodes[i]; //convenience var

		bsize = (inode->size/BLOCK_SIZE)+1;


		if( dir->inodes[i]->size % BLOCK_SIZE  ==  0 )
			bsize--;
		
		//inodes (layer 2)
		for(k = 0; k<8; k++) {
			dir->inodes[i]->data->pointers[k] = calloc(1,BLOCK_SIZE);
			block_read(super->inodes + 2 + k + (i*10),(char *)dir->inodes[i]->data->pointers[k]);
			IndexBlockLayer2 ib = dir->inodes[i]->data->pointers[k]; //convenience var
			
			//file content
			j=0;

			while( j < bsize ) {
				if( j > 511 ) {
					bsize = bsize - j;
					break;
				}

				ib->pointers[j] = calloc(1,BLOCK_SIZE);
				block_read(super->data + dataOffset,(char *)ib->pointers[j]);
				j++;
				dataOffset++;
			}
		} 
	}

  if( super->free != super->data + dataOffset) {
    errno = 5;
    perror("Read free offset inconsistent with data size!");
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
			k++;
			j++;
		}
	}
	
	
	return 0;
}

int unmount_fs(char *disk_name)
{
	printf("\nUnmounting file system...\n");
	int k,j,bsize,dataOffset=0;

	//master control block
	block_write(super->master,(char *)master);

	//file descriptors
	int i;
	for(i = 0; i<32; i++)
	{
		free(master->descriptors[i]);
	}
	
	//free index block
	IndexBlock freeb = master->free;
	block_write(super->free1,(char *)freeb);
	
	//free index blocks (layer 2)
	for(i = 0; i<8; i++)
	{
		block_write(super->free2 + i,(char *)freeb->pointers[i]);
	}
	
	//top directory
	block_write(super->dir,(char *)master->dir);
	Directory dir = master->dir;

#ifdef DEBUG
	printf("unmounting inodes...\n");
#endif
	//inodes
	for(i = 0; i<64; i++)
	{
		//printf("unmounting inode %d\n",i);
		block_write(super->inodes + (i*10),(char *)dir->inodes[i]);
		block_write(super->inodes + 1 + (i*10),(char *)dir->inodes[i]->data);

		bsize = (dir->inodes[i]->size/BLOCK_SIZE)+1;

		if( dir->inodes[i]->size % BLOCK_SIZE  ==  0 )
			bsize--;
		
		//inodes (layer 2)
		for(k = 0; k<8; k++) {
			block_write(super->inodes + 2 + k + (i*10),(char *)dir->inodes[i]->data->pointers[k]);
			IndexBlockLayer2 ib = dir->inodes[i]->data->pointers[k]; //convenience var
			
			//file content
			j=0;

			while( j < bsize ) {
				if( j > 511 ) {
					bsize = bsize - j;
					break;
				}

				block_write(super->data + dataOffset,(char *)ib->pointers[j]);
				free(ib->pointers[j]);
				j++;
				dataOffset++;
			}

			free(dir->inodes[i]->data->pointers[k]);
		} 

		free(dir->inodes[i]->data);
		free(dir->inodes[i]);
	}
	
	if( super->free != super->data + dataOffset){
		errno = 5;
		perror("unmount:Data size is inconsistent with offset value!");
	}
	
	free(master->dir);

	k=0;
	//free layer 2 free index blocks
	for(i = 0; i<8; i++)
	{

		j = 0;
		void* buf = calloc(1,BLOCK_SIZE);
		while( k < BLOCK_SIZE - dataOffset  &&  j < (BLOCK_SIZE/8) ) {
			free(freeb->pointers[i]->pointers[j]);
			block_write(super->free + k, (char *) buf);
			k++;
			j++;
		}

		free(buf);

		free(freeb->pointers[i]);
	}
	
	free(freeb);
	free(master);
	
	block_write(0,(char *)super);
	free(super);

	if( close_disk(disk_name) == -1 )
		return -1;

	active=0;

	printf("...unmount complete!\n");

	return 0;
}	

//write & open

int fs_open(char *name)
{
	//check if file exists
	int i = 0;
	int test = 0;
	//while we havent checked all files and names do not match
	while(i < 64 && test == 0) 
	{	
		if(master->dir->inodes[i]->name[0] == '\0')
			i++;//skip this "file"
		else
		{
			if(strcmp(master->dir->inodes[i]->name, name) == 0) //found file
				test = 1;
			else	//didnt find file
				i++;
		}
	}
	
	if(test == 0)
		return -1;
	
	//otherwise we found file
	//find a descriptor
	int j = 0;
	for(j = 0; j < 32; j++)
	{
		if(master->descriptors[j]->pointer == NULL) //found a empty descriptor at position j
			break;
	}
	
	if(j == 32)	//no descriptor found check
		return -1;
		
	master->descriptors[j]->pointer = master->dir->inodes[i];
	master->descriptors[j]->seek = 0;
	return j; //returns the fd number
}

int fs_create(char *name){
	Directory dir = master->dir;
	int i = 31, low = 0, high = 64; //note, i can never reach high
	int len = strlen(name);

	if( len < 1 ) {
		errno = EINVAL;
		perror("fs_create");
		return -1;
	}

	if( len > 15 ) {
		errno = ENAMETOOLONG;
		perror("fs_create");
		return -1;
	}

	if( dir->inodes[63]->name[0] != '\0' ) {
		errno = ENOMEM;
		perror("fs_create");
		return -1;
	}

	//binary search until proper position is found
	while(low != i) {
		if(strcmp(name,dir->inodes[i]->name) < 0) {
			low = i;
		}
		else if (strcmp(name,dir->inodes[i]->name) > 0) {
			high = i;
		}
		else {
			errno = EEXIST;
			perror("fs_create");
			return -1; //file already exists
		}

		i = low + (high-low)/2;
	}

	//if an exact match, notify that file exists
	if( strcmp(name,dir->inodes[i]->name) == 0 ) {
		errno = EEXIST;
		perror("fs_create");
		return -1; //file already exists
	}
	
	//special case for adding file to the end of the list
	if( dir->inodes[i]->name[0] == '\0' )
		i--;
	
	INode tmp = dir->inodes[i+1];
	INode loc = dir->inodes[63];

	strncpy(loc->name,name,len+1);
	loc->size = 0;
	sem_init(&(loc->write),0,1);
	sem_init(&(loc->read),0,0);

	dir->inodes[i+1] = loc;

	i++;

	//shift all inodes to the right
	for(; i < 63; i++) {
		loc = dir->inodes[i+1];
		dir->inodes[i+1] = tmp;
		tmp = loc;
	}
	

	return 0;
}

int fs_delete(char *name){
	Directory dir = master->dir;
	int i = 31, low = 0, high = 64; //note, i can never reach high

	//binary search until proper position is found
	while(low != i) {
		if(strcmp(name,dir->inodes[i]->name) < 0) {
			low = i;
		}
		else if (strcmp(name,dir->inodes[i]->name) > 0) {
			high = i;
		}

		i = low + (high-low)/2;
	}

	//check that the file was found
	if( strcmp(name,dir->inodes[i]->name) ) {
		errno = ENOENT;
		perror("fs_delete");
		return -1;
	}

	INode file = dir->inodes[i];

	//shift all inodes to the left
	for(; i < 63; i++) {
		dir->inodes[i] = dir->inodes[i+1];
	}

	//TODO: check semaphores for reading/writing?

	int freed = 0;
	int bsize = (file->size/BLOCK_SIZE)+1;
	int k,j;

	if( file->size % BLOCK_SIZE  ==  0 )
		bsize--;
	
	//free file content
	for(k = 0; k<8; k++) {
		IndexBlockLayer2 ib = file->data->pointers[k]; //convenience var
		
		//file content
		j=0;
		while( j < bsize ) {
			if( j > 511 ) {
				bsize = bsize - j;
				break;
			}
			free(ib->pointers[j]);
			ib->pointers[j] = NULL;
			j++;
			freed++;
		}
	}

	//reset inode and append to list
	file->size = 0;
	file->name[0] = '\0';
	dir->inodes[63] = file;
	int tfreed = freed;

	void* block = NULL;

	//insert approapriate number of free blocks into structure
	for( i = 0; i < 8  &&  freed > 0; i++ ) {
			for( j = 0; j < 512  &&  freed > 0; j++ ) {

				block = master->free->pointers[i]->pointers[j];

				if( block == NULL ) {
					void* fblock = calloc(1,BLOCK_SIZE);
					master->free->pointers[i]->pointers[j] = fblock;
					freed--;
				}
			}
	}

	super->free = super->free - tfreed;

	return 0;
}

int fs_write(int fildes, void *buf, size_t nbyte)
{
	if(master->descriptors[fildes]->pointer == NULL)
		return -1; //invalid descriptor
		
	//find our file
	INode file = master->descriptors[fildes]->pointer;
	int offset = master->descriptors[fildes]->seek;
	
	char buffer[sizeof(buf)];
	strcpy(buffer, buf);
	
	//begin writing
	int i = 0;
	int j = 4096;
	int freex = 8, freey = 512;
	int usex = 0, usey = 0;
	
	//find the position of the fd (in the blocks)
	int dec = offset;
	while(dec > 0)
	{
		dec = dec - 4096;
		if(dec > 0)
		{
			usey++;
			if(usey == 512)
			{
				usey = 0;
				usex++;
				if(usex == 8) //only occurs if we are trying to write and the file already takes up the entire disk
					return -1;
			}
		}
	} 

	int test = 1;
	while(i < nbyte && test != 0)
	{
		//check if need new block
		if(offset%4096 == 0)
		{
			while(j > 0)	//check if new block is available
			{
				if(master->free->pointers[freex-1]->pointers[freey-1] == NULL) //not pointing to a block
				{
					freey--;
					if(freey == 0)
					{
						freey = 512;
						freex--;
						if(freex == 0)
						{
							test = 0;
							break; //break the while loop
						}
					}
				}
				else //found a block that is available
				{
					usey++; //incrament block location
					if(usey == 512)	//if we hit the end of this section
					{
						usey = 0;	
						usex++;		
						if(usex == 8)	//if we are out of space
						{
							test = 0;
							break; //break the while loop
						}
					}
				}
			}
			
		}
		//TODO check that im writing correctly
		if(test != 0)
		{
			//strcpy((file->data->pointers[usex]->pointers[usey] + (offset%4096)), buffer[i]);
			//copy single char to file->data->pointers[usex]->pointer[usey] + (offset%4096)
			offset++;
			i++;
		}	
	}	
		
	//cleanup
	int written = offset - master->descriptors[fildes]->seek;
	master->descriptors[fildes]->seek = offset;
	return written;
}


//fs_close and fs_read -> need debugging
int fs_close(int fildes){
	if((master->descriptors[fildes]->pointer == NULL)||(fildes >= 32)){
		errno= 2;
		perror("No such file is open \n");
		return -1;
	}
	else
		master->descriptors[fildes]->pointer = NULL;
		master->descriptors[fildes]->seek = 0;
		return 0;	
}

int fs_read(int fildes, void *buf, size_t nbyte){
	if(master->despcriptors[fildes]->pointer == NULL || fildes > 31){
		errno = 2;
		perror("No Such file open\n");
	}

	int numbytes = atoi(nbyte);
	INode file = master->descriptors[fildes]->pointer;
	int offset = master->descriptors[fildes]->seek;
	if(numbytes > file->size - offset)
		numbytes = file->size - offset; 

	//copy data nbytes from seek to seek+nbytes to buf
	char buffer[numbytes];
	int i= 0;
	int outeroffset = offset/512;
	int inneroffset = offset%512;
	
	while(i < numbytes){
	
	//check for new block in outeroffset
	  if(inneroffset > 512){ 
		outeroffset += 1;
		inneroffset = 0;
	  }

	//copy each 4byte section to the buffer
	strcpy(buffer[i], file->data->pointers[outeroffset]->pointers[inneroffset]);


	inneroffset += 1;
	i++;
	}

	strcpy(buf, buffer);

	//cleanup
	master->descriptors[fildes]->seek += numbytes; //increment seek by nbytes

	return numbytes;

}






int fs_get_filesize(char *name){
	Directory dir = master->dir;
	int i, low = 0, high = 64; //note, i can never reach high

	//binary search until proper position is found
	for( i=31; low != i; i=low+(high-low)/2 ) {
		if(strcmp(name,dir->inodes[i]->name) < 0) {
			low = i;
		}
		else if (strcmp(name,dir->inodes[i]->name) > 0) {
			high = i;
		}
		else {
			high=i+1;
			low=i;
		}
	}

	//check that the file was found
	if( strcmp(name,dir->inodes[i]->name) ) {
		errno = ENOENT;
		perror("fs_get_filesize");
		return -1;
	}

	return dir->inodes[i]->size;
}

/**
 * Utility function intended to list the names of all files
 * @return 0 on success and -1 on error
 */
int fs_list_files() {
	Directory dir = master->dir;
	printf("\nListing directory contents...\n");
	
	int i;
	for(i=0; i<64; i++){
		if(dir->inodes[i]->name[0] != '\0')
			printf("%s\n",dir->inodes[i]->name);
#ifdef DEBUG
		else
			printf("empty\n");
#endif
	}

	printf("...end of directory content.\n\n");
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

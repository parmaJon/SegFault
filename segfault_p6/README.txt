/******************************************************
 * Filename: ReadMe
 *
 * Description: documentation for project6
 *
 * Group: SegFault
 * Authors: Lucas Stuyvesant, Garrett Ransom,
 *     Jonathan Brandt, Travis Nordquist
 * Class: CSE 325
 * Instructor: Zheng
 * Assignment: Lab Project #6
 * Assigned: April 15,2016
 * Due: May 6, 2016
******************************************************/

Description/Goals: 
The goal of this lab is to emulate a file structure, with our own design and implementation, based on what we learned in class.  Our file system should be stored on a virtual disk and should be able to store "files" of its own with read, write, open, close, delete, and similar function calls.  The system must store meta-data on the virtual disk as well.  This all is done without making any new files or calls to the kernel or OS system.  The size of the virtual disk is 8,192 blocks at 4KB each, 32KB total.  The system must also store a max of 64 files, at half the total memory of the system.  


Implementation:
The prototypes of the needed functions were provided giving a few constraints for our design, but was otherwise pretty open.  We started with a super block, a structure used to track our master control block, the number of free and used blocks, as well as a directory of those blocks, the inodes associated with them and the data that fills them.  This was used primarily when initializing the virtual disk and mounting it.  Our master control block is a struct with an array of file descriptors, and pointer to an index block and a pointer to the directory.  The directory contains an array of Inode pointers which hold the information of all the files stored on the virtual disk.  The file descriptors track the open files and contain the offset of the file cursor for the file its iNode points to.  The iNodes contain the file's name, the file's size, the semaphores for read and write to the file, and a pointer to the data of the file.  The data is stored in an Indexblock struct.  The Indexblock struct is used for three types of blocks.  The data blocks which hold the actual data stored in a file, a first level index block, which will conatain 512 pointers to level two indexblocks, and a level two indexblock which will hold 512 pointers to data blocks. Thus we have 262.144 data blocks.  The data blocks are simply void pointers for open usage.  The directory holds the iNodes for all the stored data blocks as well as holding information about the number of free blocks and used blocks.  
Once the disk is created and mounted, files can be created and stored in the virtual system.  When opening the file, the name is entered and found in the system.  It is assigned a file descriptor.  To then write or read from the file, the file is referenced by its decriptor and the bits are copied from one buffer to the other, then either printed for reading, or stored on the data block for writing.  Files are then closed by resetting the assigned descriptor to an offset of 0 and its pointer set to null.  The directory can be used to print a list of the current files.  The files can be deleted in which their datablocks are zeroed out, their iNode removed from the directory and the pointers in the Indexblocks are set back to the empty data blocks.  The files can also be truncated to a given size, or the cursor set to a desired position, and filesize can be printed from the iNode via the directory.  
 


Testing Results:
  Testing was an exhaustive process.  Once our vital functions were written and compiling we moved to the debugging stage.  We set up a test method which created a file, wrote to it, closed the file, re-opened the file and read from it.  We used valgrind to ensure no memory leaks and had it set to print what was read from the file.  We first ran with only a single character being written and read to the created file.  This worked so we added a whole sentence, at which point the program began to break.  After a while of trouble shooting, we found that the issue came from sizing a buffer using a sizeof() call instead of strlength.  Once corrected, it began to work like a charm.  We found no memory leaks and had no issues with our read, write, listing files, deleting, or truncating files.  


Bugs:
No known bugs to report

Additional Notes:


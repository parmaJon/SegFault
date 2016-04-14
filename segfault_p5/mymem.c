#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "mymem.h"
#include <time.h>


/* The main structure for implementing memory allocation.
 * You may change this to fit your implementation.
 */

typedef struct memoryElement
{
  // doubly-linked list
  struct memoryElement *prev;
  struct memoryElement *next;

  int size;            // How many bytes in this block?
  char alloc;          // 1 if this block is allocated,
                       // 0 if this block is free.
  void *ptr;           // location of block in memory pool.
}*memoryElement;

strategies myStrategy = NotSet;    // Current strategy


size_t mySize;
void *myMemory = NULL;

static struct memoryElement *head;
static struct memoryElement *tail;
static struct memoryElement *next;


/* initmem must be called prior to mymalloc and myfree.

   initmem may be called more than once in a given exeuction;
   when this occurs, all memory you previously malloc'ed  *must* be freed,
   including any existing bookkeeping data.

   strategy must be one of the following:
		- "best" (best-fit)
		- "worst" (worst-fit)
		- "first" (first-fit)
		- "next" (next-fit)
   sz specifies the number of bytes that will be available, in total, for all mymalloc requests.
*/

void initmem(strategies strategy, size_t sz)
{
	myStrategy = strategy;

	/* all implementations will need an actual block of memory to use */
	mySize = sz;

	if (myMemory != NULL) free(myMemory); /* in case this is not the first time initmem2 is called */

	/*TODO cleanup of allocated memory*/

	while(head!=NULL)
	{
		tail = head;
		head = head->next;
		free(tail);
	}

	myMemory = malloc(sz);
	
	head = malloc(sizeof(struct memoryElement));
	tail = head;
	head->size = sz;
	head->alloc = 0;
	head->ptr = myMemory;


}

/* Allocate a block of memory with the requested size.
 *  If the requested block is not available, mymalloc returns NULL.
 *  Otherwise, it returns a pointer to the newly allocated block.
 *  Restriction: requested >= 1 
 */

void *mymalloc(size_t requested)
{
	if(requested < 1)
		return NULL; //done messed up
		
	assert((int)myStrategy > 0);
	
	memoryElement trav = head;
	memoryElement best = NULL;
	memoryElement worst = NULL;
	
	memoryElement newElement = malloc(sizeof(struct memoryElement));
	newElement->size = requested;
	newElement->alloc = 1;
	
	switch (myStrategy)
	  {
	  case NotSet:
	  			free(newElement);
	            return NULL;
	            
	  case First:
	  			//traversal to first of proper size
	  			while(trav != NULL)
	  			{
	  				if(trav->alloc == 0)
	  				{
	  					if(trav->size >= requested)
	  					{
	  						if(trav->prev != NULL)
	  							trav->prev->next = newElement;
	  						else
	  							head = newElement;
	  							
	  						trav->size = trav->size - requested;
	  						newElement->next = trav;
	  						newElement->prev = trav->prev;
	  						trav->prev = newElement;
	  						newElement->ptr = trav->ptr;
	  						trav->ptr = trav->ptr + requested;
	  						
	  						if(trav->size == 0)
	  						{
	  							if(trav == tail)
	  								tail = newElement;
	  							else
	  								trav->next->prev = newElement;
	  							
	  							newElement->next = trav->next;
	  							free(trav);
	  						}
	  						return newElement->ptr;
	  					}	
	  				}
	  				trav = trav->next;
	  			}
	            return NULL;
	            
	  case Best:
	  			//find the smallest with enough size

	  			while(trav != NULL)
	  			{
	  				if(trav->alloc == 0)
	  				{
	  					if(trav->size >= requested)
	  					{
	  						if(best == NULL || trav->size < best->size)
	  							best = trav;
	  					}	
	  				}
	  				trav = trav->next;
	  			}
	  			
	  			if(best == NULL)
	  				return NULL;
	  				
	  				
	  			if(best->prev != NULL)
					best->prev->next = newElement;
				else
					head = newElement;
					
				best->size = best->size - requested;
				newElement->next = best;
				newElement->prev = best->prev;
				best->prev = newElement;
				newElement->ptr = best->ptr;
				best->ptr = best->ptr + requested;
				
				if(best->size == 0)
				{
					if(best == tail)
						tail = newElement;
					else
						best->next->prev = newElement;
					
					newElement->next = best->next;
					free(best);
				}
				return newElement->ptr;
	  			
	  case Worst:
	  			//find the largest with enough size
	  			
	  			while(trav != NULL)
	  			{
	  				if(trav->alloc == 0)
	  				{
	  					if(trav->size >= requested)
	  					{
	  						if(worst == NULL || trav->size > worst->size)
	  							worst = trav;
	  					}	
	  				}
	  				trav = trav->next;
	  			}
	  			
	  			if(worst == NULL)
	  				return NULL;
	  				
	  				
	  			if(worst->prev != NULL)
					worst->prev->next = newElement;
				else
					head = newElement;
					
				worst->size = worst->size - requested;
				newElement->next = worst;
				newElement->prev = worst->prev;
				worst->prev = newElement;
				newElement->ptr = worst->ptr;
				worst->ptr = worst->ptr + requested;
				
				if(worst->size == 0)
				{
					if(worst == tail)
						tail = newElement;
					else
						worst->next->prev = newElement;
					
					newElement->next = worst->next;
					free(worst);
				}
				return newElement->ptr;

	  case Next:
	  			//repeat fist but start at next
	  			trav = next;
	  			
	  			if(trav->alloc == 0)
  				{
  					if(trav->size >= requested)
  					{
  						if(trav->prev != NULL)
  							trav->prev->next = newElement;
  						else
  							head = newElement;
  							
  						trav->size = trav->size - requested;
  						newElement->next = trav;
  						newElement->prev = trav->prev;
  						trav->prev = newElement;
  						newElement->ptr = trav->ptr;
  						trav->ptr = trav->ptr + requested;
  						
  						if(trav->size == 0)
  						{
  							if(trav == tail)
  								tail = newElement;
  							else
  								trav->next->prev = newElement;
  							
  							newElement->next = trav->next;
  							free(trav);
  						}
  						next = newElement->next;
  						return newElement->ptr;
  					}	
  				}
	  			
	  			trav = trav->next;
	  			
	  			if(trav == NULL)
	  				trav = head;
	  			
	  			while(trav != next)
	  			{
	  				if(trav->alloc == 0)
	  				{
	  					if(trav->size >= requested)
	  					{
	  						if(trav->prev != NULL)
	  							trav->prev->next = newElement;
	  						else
	  							head = newElement;
	  							
	  						trav->size = trav->size - requested;
	  						newElement->next = trav;
	  						newElement->prev = trav->prev;
	  						trav->prev = newElement;
	  						newElement->ptr = trav->ptr;
	  						trav->ptr = trav->ptr + requested;
	  						
	  						if(trav->size == 0)
	  						{
	  							if(trav == tail)
	  								tail = newElement;
	  							else
	  								trav->next->prev = newElement;
	  							
	  							newElement->next = trav->next;
	  							free(trav);
	  						}
	  						next = newElement->next;
	  						return newElement->ptr;
	  					}	
	  				}
	  				
	  				trav = trav->next;
	  				
	  				if(trav == NULL)
	  					trav = head;
	  				
	  			}
	  			
	            return NULL;
	            
	  }
	return NULL;
}


/* Frees a block of memory previously allocated by mymalloc. */
void myfree(void* block)
{
	//free a block thats in use
	if(head == NULL)
		return;
	
	memoryElement trav = head;
	memoryElement temp = NULL;
	
	//finds our block
	while(trav != NULL && !(trav->ptr <= block && block <= trav->ptr + trav->size))
	{
		trav = trav->next;
	}
	
	//if block is never found in the memory
	if(trav == NULL)
		return;
	
	temp = trav;
	
	//if prev element exists and is not alloced
	if(temp->prev != NULL && temp->prev->alloc == 0)
	{
		//do prep to free temp
		temp->prev->next = temp->next;
		
		//if there is an element after temp
		if(temp->next != NULL)
		{
			temp->next->prev = temp->prev;
			//if the next element needs to be freed
			if(temp->next->alloc == 0)
			{
				//if the element is not the null
				if(temp->next->next != NULL)
					temp->next->next->prev = temp->prev;
					
				temp->prev->size += temp->next->size;
				temp->prev->next = temp->next->next;
				free(temp->next);
			}
		}
		temp->prev->size += temp->size;
		temp = temp->prev;
		free(temp->next);
	}
	
	//if next element exists and is not alloced
	else if(temp->next != NULL && temp->prev->alloc == 0)
	{	
		//trav will be used for the next element
		trav = temp -> next;
		temp->next = temp->next->next;
		
		if(temp->next != NULL)
			temp->next->prev = temp;
		
		temp->size += trav->size;
		free(trav);
		temp->alloc = 0;
	}
	
	//else no other elements around this block are un alloced
	else
		temp->alloc = 0;
	
	if(temp->prev == NULL)
		head = temp;
	if(temp->next == NULL)
		tail = temp;
	
	return;
}

/****** Memory status/property functions ******
 * Implement these functions.
 * Note that when we refer to "memory" here, we mean the 
 * memory pool this module manages via initmem/mymalloc/myfree. 
 */

/* Get the number of contiguous areas of free space in memory. */
int mem_holes()
{
	return 0;
}

/* Get the number of bytes allocated */
int mem_allocated()
{
    struct memoryElement *check = head; //assigns the start of the memlist for checking
    int allocated = 0;

    while(check){ //loops through memlist blocks

	if(check->alloc == '1'){   //checks if current block is allocated
	allocated = allocated + (check->size);  //adds the allocated block's size
	}
	check = check->next;
    }

    return allocated;
}

/* Number of non-allocated bytes */
/* Functions same as mem_allocated except counts blocks marked free */
int mem_free()
{
    struct memoryElement *check = head;
    int free = 0;

    while(check){
        if(check->alloc == '0'){
	free = free + (check->size);
	}
	check = check->next;
    }

    return free;
}

/* Number of bytes in the largest contiguous area of unallocated memory */
int mem_largest_free()
{
	return 0;
}

/* Number of free blocks smaller than "size" bytes. */
int mem_small_free(int size)
{
	return 0;
}       

char mem_is_alloc(void *ptr)
{
        return 0;
}

/* 
 * Feel free to use these functions, but do not modify them.  
 * The test code uses them, but you may ind them useful.
 */


//Returns a pointer to the memory pool.
void *mem_pool()
{
	return myMemory;
}

// Returns the total number of bytes in the memory pool. */
int mem_total()
{
	return mySize;
}


// Get string name for a strategy. 
char *strategy_name(strategies strategy)
{
	switch (strategy)
	{
		case Best:
			return "best";
		case Worst:
			return "worst";
		case First:
			return "first";
		case Next:
			return "next";
		default:
			return "unknown";
	}
}

// Get strategy from name.
strategies strategyFromString(char * strategy)
{
	if (!strcmp(strategy,"best"))
	{
		return Best;
	}
	else if (!strcmp(strategy,"worst"))
	{
		return Worst;
	}
	else if (!strcmp(strategy,"first"))
	{
		return First;
	}
	else if (!strcmp(strategy,"next"))
	{
		return Next;
	}
	else
	{
		return 0;
	}
}


/* 
 * These functions are for you to modify however you see fit.  These will not
 * be used in tests, but you may find them useful for debugging.
 */

/* Use this function to print out the current contents of memory. */
void print_memory()
{
	return;
}

/* Use this function to track memory allocation performance.  
 * This function does not depend on your implementation, 
 * but on the functions you wrote above.
 */ 
void print_memory_status()
{
	printf("%d out of %d bytes allocated.\n",mem_allocated(),mem_total());
	printf("%d bytes are free in %d holes; maximum allocatable block is %d bytes.\n",mem_free(),mem_holes(),mem_largest_free());
	printf("Average hole size is %f.\n\n",((float)mem_free())/mem_holes());
}

/* Use this function to see what happens when your malloc and free
 * implementations are called.  Run "mem -try <args>" to call this function.
 * We have given you a simple example to start.
 */
void try_mymem(int argc, char **argv) {
        strategies strat;
	void *a, *b, *c, *d, *e;
	if(argc > 1)
	  strat = strategyFromString(argv[1]);
	else
	  strat = First;
	
	
	/* A simple example.  
	   Each algorithm should produce a different layout. */
	
	initmem(strat,500);
	
	a = mymalloc(100);
	b = mymalloc(100);
	c = mymalloc(100);
	myfree(b);
	d = mymalloc(50);
	myfree(a);
	e = mymalloc(25);
	
	print_memory();
	print_memory_status();
	
}

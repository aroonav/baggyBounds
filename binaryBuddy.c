#include <stdio.h>
#include <stdlib.h>

#define TOTAL_MEMORY 1024*1024	//1024K bytes
#define SLOT_SIZE 64*1024		//64K bytes

int logbase2 (int num)
{
	int i = 0;
	while (num != 1)
	{
		num = num/2;
		i++;
	}
	return i;
}

typedef struct node
{
	void* mptr;
	struct node* next;
	int used;			//0-Not used, 1->used block or the block is divided into two smaller blocks
}mem_block;

/* Global Variables */
int log_total_mem;
int log_slot_mem;
int no_of_heads;
mem_block** heads;

/*
If TOTAL_MEMORY=1024K and SLOT_SIZE=64K then no_of_heads=5
heads[i] denotes the series of free memory blocks where each block
is a size of (2^i)*SLOT_SIZE.
*/

int init()
{
	log_total_mem = logbase2(TOTAL_MEMORY);
	log_slot_mem = logbase2(SLOT_SIZE);
	no_of_heads = log_total_mem - log_slot_mem + 1;
	heads = malloc(no_of_heads*sizeof(mem_block*));

	for (int i = 0; i < no_of_heads; ++i)
		heads[i]=NULL;
	mem_block* initial_node = malloc(sizeof(mem_block));
	initial_node->mptr = malloc(TOTAL_MEMORY);
	initial_node->next = NULL;
	initial_node->used = 0;

	heads[no_of_heads-1] = initial_node;

	return 0;
}

void show_memory()
{
	for (int i = no_of_heads-1; i >= 0; i--)
	{
		mem_block* temp = heads[i];
		size_t blocksize = (SLOT_SIZE<<i)/1024;	//In Kilo Bytes
		printf("Block size:%luK\n", blocksize);
		while(temp!=NULL)
		{
			printf("%d(%p-%p)->",temp->used, temp->mptr, ((temp->mptr)+(blocksize<<10)));
			temp = temp->next;
		}
		printf("NULL\n");
	}
}

// This function returns a generic pointer to the memory block.
void* bmalloc(size_t size)
{
	int i;
	if(size>TOTAL_MEMORY)
	{
		fprintf(stderr, "Not enough memory to allocate.\n");
		return NULL;
	}
	int index;
	if(size<SLOT_SIZE)
		index=0;
	else
		index = logbase2(size)-log_slot_mem+1;		// We need a memory block from heads[index] or above.

	// Find the correct level where there is sufficient memory for the requested memory block.
	mem_block* target = NULL;
	for (i = index; i < no_of_heads; i++)
	{
		if (heads[i]==NULL)							// There are no free blocks of memory at this level.
		{
			continue;
		}
		else
		{
			target = heads[i];
			while(target!=NULL && target->used==1)	// Go through all the used up blocks till NULL comes
				target=target->next;
			if(target==NULL)
				continue;
			else
				break;
		}
	}
	// Break a big block of memory node pointed by target into two parts.
//	printf("@@@@@@@@@@@@@@@%p %d\n", heads[i], index);
	for (int j = i; j > index; j--)
	{
		if((j-1) >= 0)	// A bigger block needs to be divided into two smaller blocks
		{
			size_t new_block_size = SLOT_SIZE<<(j-1);
			mem_block* block1 = malloc(sizeof(mem_block));		// Left block
			mem_block* block2 = malloc(sizeof(mem_block));		// Right block
			block1->mptr = target->mptr;
			block1->next = block2;
			block2->mptr = block1->mptr + new_block_size;
			block2->next = NULL;
			target->used = 1;		// Bigger block has been successfully divided into two smaller blocks.

			mem_block* temp = heads[j-1];
			if(temp==NULL)				// There is no node at the linked list in heads[j-1]
			{
				heads[j-1] = block1;
				block2->next = NULL;
			}
			else						// There's atleast one node at the linked list in heads[j-1]
			{
				if(temp->next==NULL)	// There's exactly one node
				{
					if(temp->mptr+new_block_size < block1->mptr)
					{
						temp->next = block1;
						block2->next = NULL;
					}
					else if(block2->mptr+new_block_size < temp->mptr)
					{
						heads[j-1] = block1;
						block2->next = temp;
					}
				}
				else	// We need to loop to find the correct position for the pair of new memory blocks
				{
					// The correct position of block1 and block2 is between temp and temp->next
					while(temp->mptr < block1->mptr && temp->next->mptr > (block2->mptr + new_block_size))
					{
						temp=temp->next;
					}
					block2->next = temp->next;
					temp->next = block1;
				}
			}
		}
	}
	target = heads[index];								// First block of size SLOT_SIZE*(2^j)
	while(target!=NULL && target->used != 0)			// Find an unused block to split
		target=target->next;
	if(target!=NULL)
	{
		target->used=1;
		return target->mptr;
	}
	else
		return NULL;
}

int free_all()
{
	for (int i = no_of_heads-1; i>=0; i--)
	{
		mem_block* node = heads[i];
		while(node!=NULL)
		{
			free(node->mptr);
			node->mptr = NULL;
			node = node->next;
		}
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	init();
	printf("\nMemory:\n");show_memory();
	printf("Allocated memory in:%p\n", bmalloc(10*1024));
	printf("\nMemory:\n");show_memory();
	// printf("Allocated memory in:%p\n", bmalloc(10*1024));
	// printf("\nMemory:\n");show_memory();
	// printf("Allocated memory in:%p\n", bmalloc(10*1024));
	// printf("\nMemory:\n");show_memory();

/*
	free_all();
	for(int i=no_of_heads-1; i>=0; i--)
	{
		printf("%p ", heads[i]);
		if(heads[i]!=NULL)
			printf("%p\n", heads[i]->mptr);
		else	printf("\n");
	}
*/
	return 0;
}
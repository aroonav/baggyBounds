#include <stdio.h>
#include <stdlib.h>

#define TOTAL_MEMORY 1024*1024	//1024K bytes
#define SLOT_SIZE 64*1024		//64K bytes

typedef struct node
{
	void* mptr;
	struct node* next;
	int used;			//0-Not used, 1->used block or the block is divided into two smaller blocks
}mem_block;

int logbase2 (int num);
int init();
void show_memory();
void* bmalloc(size_t size);
int bfree(void *p);

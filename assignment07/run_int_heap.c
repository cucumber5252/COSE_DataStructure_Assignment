#include <stdio.h>
#include <stdlib.h> // malloc, rand, free
#include <time.h> // time

#include "adt_heap.h"

#define MAX_ELEM	20

/* user-defined compare function */
int compare(const void *arg1, const void *arg2)
{
	int *a1 = (int *)arg1;
	int *a2 = (int *)arg2;
	
	return *a1 - *a2;
}

////////////////////////////////////////////////////////////////////////////////
/* user-defined print function */
void print_func(const void *data)
{
	printf(" %4d", *(int *)data);
}

////////////////////////////////////////////////////////////////////////////////
void remove_data(void *data)
{
	free(data);
}

////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	HEAP *heap;
	int data;
	void *dataPtr;
	int i;
	
	heap = heap_Create(compare);
	
	srand(time(NULL));
	
	printf("Insert:");
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // 1 ~ MAX_ELEM*3 random number
		
		fprintf(stdout, " %4d", data);
		
		int *newdata = (int *)malloc( sizeof(int));
		*newdata = data;
		
		// insert function call
		if (heap_Insert(heap, newdata) == 0) break;
 	}
	printf("\n");
	
	printf( "Heap:  ");
	heap_Print(heap, print_func);
	
	printf( "Delete:");
	
	while (!heap_Empty( heap))
	{
		// delete function call
		heap_Delete(heap, &dataPtr);

		printf(" %4d", *(int *)dataPtr);

		remove_data(dataPtr);
 	}
	printf("\n");
	
	heap_Destroy(heap, remove_data);

	return 0;
}

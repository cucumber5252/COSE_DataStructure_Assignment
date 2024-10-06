typedef struct
{
	int	last;
	int	capacity;
	void **heapArr;
	int (*compare) (const void *, const void *);
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
The initial capacity of the heap should be 10
*/
HEAP *heap_Create( int (*compare) (const void *arg1, const void *arg2));

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap, void (*remove_data)(void *ptr));

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr);

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr);

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap);

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (const void *data));


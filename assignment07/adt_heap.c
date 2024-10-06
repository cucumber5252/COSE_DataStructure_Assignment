#include <stdio.h>
#include <stdlib.h> // malloc, realloc, free

#include "adt_heap.h"
static void _reheapUp(HEAP *heap, int index);
static void _reheapDown(HEAP *heap, int index);

// Reestablishes heap by moving data in child up to correct location heap array for heap_Insert function
static void _reheapUp(HEAP *heap, int index) {
    int parent;
    void *temp;

    if (index) {
        parent = (index - 1) / 2;
        if (heap->compare(heap->heapArr[index], heap->heapArr[parent]) > 0) {
            temp = heap->heapArr[index];
            heap->heapArr[index] = heap->heapArr[parent];
            heap->heapArr[parent] = temp;
            _reheapUp(heap, parent);
        }
    }
}

// Reestablishes heap by moving data in root down to its correct location in the heap for heap_Delete function
static void _reheapDown(HEAP *heap, int index) {
    void *temp;
    int leftChild, rightChild, largerChild;

    leftChild = 2 * index + 1;
    rightChild = 2 * index + 2;

    if (leftChild < heap->last) {
        if (rightChild < heap->last && heap->compare(heap->heapArr[rightChild], heap->heapArr[leftChild]) > 0) {
            largerChild = rightChild;
        } else {
            largerChild = leftChild;
        }

        if (heap->compare(heap->heapArr[largerChild], heap->heapArr[index]) > 0) {
            temp = heap->heapArr[index];
            heap->heapArr[index] = heap->heapArr[largerChild];
            heap->heapArr[largerChild] = temp;
            _reheapDown(heap, largerChild);
        }
    }
}

// Allocates memory for heap and returns address of heap head structure if memory overflow, NULL returned
HEAP *heap_Create(int (*compare)(const void *arg1, const void *arg2)) {
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
    if (!heap) return NULL;

    heap->last = 0;
    heap->capacity = 10;
    heap->compare = compare;
    heap->heapArr = (void **)malloc(heap->capacity * sizeof(void *));
    if (!heap->heapArr) {
        free(heap);
        return NULL;
    }

    return heap;
}

// Free memory for heap
void heap_Destroy(HEAP *heap, void (*remove_data)(void *ptr)) {
    if (heap) {
        for (int i = 0; i < heap->last; i++) {
            remove_data(heap->heapArr[i]);
        }
        free(heap->heapArr);
        free(heap);
    }
}

// Inserts data into heap, return 1 if successful; 0 if heap full
int heap_Insert(HEAP *heap, void *dataPtr) {
    void **temp;

    if (heap->last == heap->capacity) {
        temp = (void **)realloc(heap->heapArr, 2 * heap->capacity * sizeof(void *));
        if (!temp) return 0;

        heap->heapArr = temp;
        heap->capacity *= 2;
    }

    heap->heapArr[heap->last] = dataPtr;
    _reheapUp(heap, heap->last);
    (heap->last)++;
    return 1;
}

// Deletes root of heap and passes data back to caller, return 1 if successful; 0 if heap empty
int heap_Delete(HEAP *heap, void **dataOutPtr) {
    if (heap->last == 0) return 0;

    *dataOutPtr = heap->heapArr[0];
    
    heap->heapArr[0] = heap->heapArr[heap->last];
    _reheapDown(heap, 0);
    (heap->last)--;
    return 1;
}

// Return 1 if the heap is empty; 0 if not
int heap_Empty(HEAP *heap) {
    return heap->last == 0;
}

// Print heap array
void heap_Print(HEAP *heap, void (*print_func)(const void *data)) {
    for (int i = 0; i < heap->last; i++) {
        print_func(heap->heapArr[i]);
    }
    printf("\n");
}
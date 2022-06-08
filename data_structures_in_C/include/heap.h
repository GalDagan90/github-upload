/****************************************************************************
*Author : Gal Dagan
*Date : 22.09.21
*Reviewer : Ido Sapan
*
*Description : Implementation of heap data structure.
*
*Lab : ol108
****************************************************************************/
#ifndef __HEAP_H_OL108_ILRD__
#define __HEAP_H_OL108_ILRD__

#include <stddef.h> /* size_t */

typedef struct heap heap_t;

/**
 * @Description: Pointer to a function that compares between two values
 *               of a data element.
 * @Parameters: pointers to two elements to compare.
 * @Return: Zero if they are equal, a negative number if node_data should be above
 *      cmp_data. Positive number otherwise.
**/
typedef int (*heap_cmp_t)(const void *node_data, const void *cmp_data);

/**
 * @Description: Pointer to a function that checks if the data of two elements
 *               is the same.
 * @Parameters: Two void pointers to the data to compare.
 * @Return: One if they are equal, zero if not.
**/
typedef int (*is_match_heap_t)(const void *node_data, const void *cmp_data);

/**
 * @Description: Creates an empty heap.
 * @Parameters: cmp_func - function to compare the data of different data elements.
 * @Return: a pointer to the new heap.
 * @Complexity: O(1).
**/
heap_t *HeapCreate(heap_cmp_t cmp_func);

/**
 * @Description: Destroys the heap.
 * @Parameters: heap - a pointer to the heap to be destroyed.
 * @Return: void.
 * @Complexity: O(1).
**/
void HeapDestroy(heap_t *heap);

/**
 * @Description: Inserts data element with given data to the heap.
 * @Parameters: heap - pointer to the heap.
 *				data - pointer to the data of the data element to be inserted.
 * @Return: 0 if inserted successfuly, non-zero otherwise.
 * @Complexity: O(log(n)), n - number of elements in the heap.
**/
int HeapPush(heap_t *heap, const void *data);

/**
 * @Description: Removes the first element in the heap.
 * @Parameters: heap - pointer to the heap.
 * @Return: void pointer to the popped data, NULL if heap is empty.
 * @Complexity: O(log(n)), n - number of elements in the heap.
**/
void *HeapPop(heap_t *heap); /* return *(void **)VectorGetData();   */

/**
 * @Description: Peek to the first element in the heap.
 * @Parameters: heap - pointer to the heap.
 * @Return: void pointer to the data, NULL if heap is empty.
 * @Complexity: O(1).
**/
void *HeapPeek(const heap_t *heap); /* return *(void **)VectorGetData();   */

/**
 * @Description: Removes a given element in the heap.
 * @Parameters: heap - pointer to the heap.
 *              to_del - pointer to the element to be removed.
 *              is_match - pointer to a function that checks if two elements
 *                      are the same.
 * @Return: void pointer to the removed data, NULL if to_del is not in
 *          the heap.
 * @Complexity: O(n), n - number of elements in the heap.
**/
void *HeapRemove(heap_t *heap, const void *to_del, is_match_heap_t is_match);

/**
 * @Description: Check if the heap is empty.
 * @Parameters: heap - pointer to the heap.
 * @Return: 1 - if heap is empty, 0 -otherwise.
 * @Complexity: O(1).
**/
int HeapIsEmpty(const heap_t *heap);

/**
 * @Description: Returns the number of elements currently in the heap.
 * @Parameters: heap - pointer to the heap.
 * @Return: Number of elements in the heap
 * @Complexity: O(1).
**/
size_t HeapSize(const heap_t *heap);

#endif /* __HEAP_H_OL108_ILRD__ */
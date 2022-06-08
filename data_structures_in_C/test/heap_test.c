/****************************************************************************
*Author : Gal Dagan
*Date : 22.09.21
*Reviewer : Ido Sapan
*
*Description : Implementation of heap data structure.
*
*Lab : ol108
****************************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "heap.h"

#define UNUSED(x) (void)(x)
#define ARR_SIZE 10

int HeapCmp(const void *node_data, const void *cmp_data);
int IsMatch(const void *node_data, const void *cmp_data);
static void TestHeapPush(heap_t *heap, int *arr);
static void TestHeapPop(heap_t *heap);
static void TestHeapRemove(heap_t *heap);

int main(void)
{
	heap_t *heap = HeapCreate(&HeapCmp);
	int arr[ARR_SIZE] = {12,3,-5,0,9,102,54,1,22,-36};
	
	assert(NULL != heap);
	assert(HeapIsEmpty(heap));
	assert(0 == HeapSize(heap));
	
	TestHeapPush(heap, arr);
	TestHeapPop(heap);
	TestHeapRemove(heap);

	HeapDestroy(heap);

	puts("Tests Done!");

	return 0;
}

int HeapCmp(const void *node_data, const void *cmp_data)
{
	assert(NULL != node_data);
	assert(NULL != cmp_data);
	
	return (*(int *)node_data - *(int *)cmp_data);
}

int IsMatch(const void *node_data, const void *cmp_data)
{
	assert(NULL != node_data);
	assert(NULL != cmp_data);
	
	return (*(int *)node_data == *(int *)cmp_data);
}

static void TestHeapPush(heap_t *heap, int *arr)
{
	int i = 0;

	HeapPush(heap, &arr[i]);
	assert(12 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(3 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-5 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
	HeapPush(heap, &arr[i]);
	assert(-36 == *(int *)HeapPeek(heap));
	assert((size_t)(i + 1) == HeapSize(heap));
	++i;
}

static void TestHeapPop(heap_t *heap)
{
	int i = 1;
	size_t initial_size = HeapSize(heap);

	for (;i < ARR_SIZE; ++i)
	{
		int data = *(int *)HeapPop(heap);
		assert(HeapSize(heap) == initial_size - i);
		assert(data <= *(int *)HeapPeek(heap));
	}

	HeapPop(heap);
	assert(0 == HeapSize(heap));
	assert(HeapIsEmpty(heap));
}

static void TestHeapRemove(heap_t *heap)
{
	int arr[] = {-30,5,9,10,7,19,101,13,15,8};
	int i = 0;
	int x = 0;
	int not_in_arr = 99;

	for (; i < ARR_SIZE; ++i)
	{
		HeapPush(heap, &arr[i]);
	}

	assert(ARR_SIZE == HeapSize(heap));
	assert(-30 == *(int *)HeapPeek(heap));

	x = (*(int *)HeapRemove(heap, &arr[7], &IsMatch));
	assert(13 == x);
	assert(*(int *)HeapPeek(heap) == -30);
	assert(ARR_SIZE - 1 == HeapSize(heap));

	x = (*(int *)HeapRemove(heap, &arr[0], &IsMatch));
	assert(-30 == x);
	assert(*(int *)HeapPeek(heap) == 5);
	assert(ARR_SIZE - 2 == HeapSize(heap));

	assert(NULL == HeapRemove(heap, &not_in_arr, &IsMatch));
	assert(ARR_SIZE - 2 == HeapSize(heap));
}
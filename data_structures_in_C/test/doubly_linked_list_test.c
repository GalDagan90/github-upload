#include <stdio.h> 	/* printf() */
#include <assert.h>	/* assert() */

#include "doubly_linked_list.h"

#define UNUSED(var) ((void)var)
#define ARR_SIZE 10

int PrintIterData(void *data, void *param);
int CompareIters(const void *data1, const void *data2);

int main()
{
	dlist_t *dlist = NULL;
	dlist_t *dlist_2 = NULL;
	dlist_iter_t iter1;
	dlist_iter_t iter2;
	/*dlist_iter_t dest;*/
	int arr[ARR_SIZE] = {10, 5, -56, 24, 5, 963, 10, 555, -444, 666};
	operation_func_t print_func = &PrintIterData;
	is_match_t is_match = &CompareIters;
	void *empty = NULL;
	
	dlist = DListCreate();
	dlist_2 = DListCreate();
	
	/*printf("Dlist Size after create: %lu\n", DListGetSize(dlist));*/
	
	/*iter1 = DListGetHead(dlist);
	DListInsertBefore(iter1, &arr[0]);
	iter1 = DListGetHead(dlist);
	printf("%d\n", *(int *)DListGetData(iter1));
	
	iter2 = DListGetEnd(dlist);
	DListInsertBefore(iter2, &arr[1]);
	iter2 = DListGetNext(iter1);
	printf("%d\n", *(int *)DListGetData(iter2));
	
	printf("Dlist Size after insertion: %lu\n", DListGetSize(dlist));
	
	DListRemove(iter2);
	printf("%d\n", *(int *)DListGetData(iter1));
	printf("Dlist Size after Remove: %lu\n", DListGetSize(dlist));
	
	PushBack(dlist, &arr[2]);
	printf("Dlist Size after PushBack: %lu\n", DListGetSize(dlist));
	PushFront(dlist, &arr[3]);
	printf("Dlist Size after PushFront: %lu\n", DListGetSize(dlist));
	iter1 = DListGetHead(dlist);
	printf("%d\n", *(int *)DListGetData(iter1));
	PopBack(dlist);
	printf("Dlist Size after PopBack: %lu\n", DListGetSize(dlist));
	PopFront(dlist);
	printf("Dlist Size after PopBack: %lu\n", DListGetSize(dlist));*/
	
	DListPushBack(dlist, &arr[0]);
	DListPushBack(dlist, &arr[1]);
	DListPushBack(dlist, &arr[2]);
	DListPushFront(dlist, &arr[3]);
	DListPushBack(dlist, &arr[4]);
	DListPushFront(dlist, &arr[5]);
	DListPopBack(dlist);
	printf("Dlist Size after Push: %lu\n", DListGetSize(dlist));
	
	iter1 = DListGetHead(dlist);
	iter2 = DListGetEnd(dlist);
	DListForEach(iter1, iter2, print_func, empty);
	
	iter1 = DListFind(iter1, iter2, &arr[8] ,is_match);
	printf("%d\n", NULL == DListGetData(iter1));
	/*DListMultiFind(iter1, iter2, &arr[1],is_match, dlist_2);
	printf("Dlist_2 Size after MultiFind: %lu\n", DListGetSize(dlist_2));
	printf("Dlist Size after MultiFind: %lu\n", DListGetSize(dlist));*/
	
	/*iter2 = DListGetNext(iter1);
	iter2 = DListGetNext(iter2);*/
	
	/*dest = DListGetHead(dlist_2);*/
	/*iter1 = DListFind(iter1, iter2, &arr[3] ,is_match);*/
	/*printf("%d\n", NULL == DListGetData(iter1));*/
	
	/*DListSplice(iter1,iter2, dest);
	printf("Dlist Size after Splice: %lu\n", DListGetSize(dlist));
	printf("Dlist_2 Size after Splice: %lu\n", DListGetSize(dlist_2));*/
	
	iter1 = DListGetHead(dlist_2);
	iter2 = DListGetEnd(dlist_2);
	DListForEach(iter1, iter2, print_func, empty);
	
	DListDestroy(dlist);
	DListDestroy(dlist_2);
	
	return (0);
}

int PrintIterData(void *data, void *param)
{
	UNUSED(param);
	printf("%d\n", *(int *)data);
	
	return (0);
}

int CompareIters(const void *data1, const void *data2)
{	
	assert(NULL != data1);
	assert(NULL != data2);
	
	return (*(int *)data1 == *(int *)data2);
}

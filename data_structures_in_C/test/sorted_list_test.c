#include <stdio.h> 	/* printf() */
#include <assert.h>	/* assert() */

#include "sorted_list.h"

#define UNUSED(var) ((void)var)
#define ARR_SIZE 5

int CompareFunction(const void *data_1, const void *data_2);
int PrintIterData(void *data, void *param);

static void PrintTestRes(int status);
static void TestSize(sort_list_t *list);
static void TestInsert(sort_list_t *list, int *arr);
static void TestGetBegin(sort_list_t *list, int *arr);
static void TestGetEnd(sort_list_t *list);
static void TestGetNext(sort_list_t *list, int *arr);
static void TestRemove(sort_list_t *list);
static void TestFind(sort_list_t *list);
static void TestMerge(sort_list_t *list1, sort_list_t *list2);

int main()
{
	int arr[ARR_SIZE] = {100, 50, 60, 47, 9};
	int arr_2[ARR_SIZE] = {1, 12, 3, 84, -12};
	sort_cmp_func_t cmp_func = CompareFunction;
	sort_list_t *sort_list = NULL;
	sort_list_t *sort_list_2 = NULL;
	operation_func_t print_func = &PrintIterData;
	void *empty = NULL;
	
	sort_list = SortListCreate(cmp_func);
	sort_list_2 = SortListCreate(cmp_func);
	
	printf("Test Size: "); 
	TestSize(sort_list);
	printf("Insert: "); 
	TestInsert(sort_list, arr);
	printf("Insert: "); 
	TestInsert(sort_list_2, arr_2);
	printf("GetBegin: "); 
	TestGetBegin(sort_list, arr);
	printf("GetEnd: "); 
	TestGetEnd(sort_list);
	printf("GetNext: "); 
	TestGetNext(sort_list, arr);
	printf("Remove: "); 
	TestRemove(sort_list);
	
	SortListDestroy(sort_list);
	sort_list = SortListCreate(cmp_func);
	printf("Insert: "); 
	TestInsert(sort_list, arr);
	
	printf("Find: "); 
	TestFind(sort_list);
	printf("Merge: "); 
	TestMerge(sort_list_2, sort_list);
	
	SortListForEach(SortListGetBegin(sort_list_2), SortListGetEnd(sort_list_2),
					print_func, empty);
	
	SortListDestroy(sort_list);
	SortListDestroy(sort_list_2);

	return (0);
}

static void PrintTestRes(int status)
{
	(status == 1)? printf("Success\n"): printf("failure\n");
}

static void TestSize(sort_list_t *list)
{	
	size_t size = SortListSize(list);
	
	(size == 0 || size == ARR_SIZE)? PrintTestRes(1): PrintTestRes(0);
}

static void TestInsert(sort_list_t *list, int *arr)
{
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		SortListInsert(list, &arr[i]);
	}
	
	(SortListSize(list) == 5)? PrintTestRes(1): PrintTestRes(0);
}

static void TestGetBegin(sort_list_t *list, int *arr)
{
	void *data = SortListGetData(SortListGetBegin(list));
	
	(CompareFunction(data, &arr[4]) == 0)? PrintTestRes(1): PrintTestRes(0);
}

static void TestGetEnd(sort_list_t *list)
{
	void *data = SortListGetData(SortListGetEnd(list));
	
	(data == NULL)? PrintTestRes(1): PrintTestRes(0);
}

static void TestGetNext(sort_list_t *list, int *arr)
{
	sort_list_iter_t iter = SortListGetBegin(list);
	void *data = SortListGetData(SortListGetNext(iter));
	
	(CompareFunction(data, &arr[3]) == 0)? PrintTestRes(1): PrintTestRes(0);
}

static void TestRemove(sort_list_t *list)
{
	SortListRemove(SortListGetPrev(SortListGetEnd(list)));
	SortListPopBegin(list);
	SortListPopEnd(list);
	
	(SortListSize(list) == 2)? PrintTestRes(1): PrintTestRes(0);
}

static void TestFind(sort_list_t *list)
{
	int x = 55;
	sort_list_iter_t iter1 = SortListFind(list, &x);
	
	(NULL == SortListGetData(iter1))? PrintTestRes(1): PrintTestRes(0);
}

static void TestMerge(sort_list_t *list1, sort_list_t *list2)
{
	SortListMerge(list1, list2);
	(SortListSize(list1) == 10 && SortListSize(list2) == 0)? 
		PrintTestRes(1): PrintTestRes(0);
}

int CompareFunction(const void *data_1, const void *data_2)
{
	return (*(int *)data_1 - *(int *)data_2);
}

int PrintIterData(void *data, void *param)
{
	UNUSED(param);
	printf("%d\n", *(int *)data);
	
	return (0);
}

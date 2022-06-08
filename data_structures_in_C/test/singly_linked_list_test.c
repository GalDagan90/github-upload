#include <stdio.h> 	/* printf() */
#include <assert.h>	/* assert() */

#include "singly_linked_list.h"

#define ARR_SIZE 10

enum status
{
	SUCCESS = 0,
	FAILURE
};

static void PrintTestResult(int status);
static void TestSListCreate(slist_t *list);
static void TestIsListEmpty(slist_t *list, size_t num_of_elements);
static void TestSListGetSize(slist_t *list, size_t num_of_elements);
static void TestSListGetHead(slist_t *list, slist_iter_t *iter, int *arr);
static void TestSListGetEnd(slist_t *list, slist_iter_t *iter, int *arr);
static void TestSListGetNext(slist_t *list, slist_iter_t *iter, int *arr);
static void TestSListInsertBefore(slist_t *list, slist_iter_t *iter, int *arr);
static void TestSListRemove(slist_t *list);
static void TestSListIsIterEqual(slist_iter_t *iter1, slist_iter_t *iter2); 
static void TestSListForEach(slist_iter_t *iter_from, slist_iter_t *iter_to, 
							int *arr);
int PrintIterData(void *data);
int MultiplyIterData(void *data);
int CompareIters(void *data1, void *data2);
							
int main()
{
	slist_t *linked_list = NULL;
	slist_iter_t *iter1 = NULL;
	slist_iter_t *iter2 = NULL;
	operation_func_ptr multi_f_ptr = &MultiplyIterData;
	int arr[ARR_SIZE] = {1, 5, -56, 24, -245, 963, 10, 555, -444, 666};
	
	linked_list = SListCreate();
	
	/* Test before insertion to the linked list */
	TestSListCreate(linked_list);
	TestIsListEmpty(linked_list, 0);
	TestSListGetSize(linked_list, 0);
	TestSListGetHead(linked_list, iter1, arr);
	TestSListGetEnd(linked_list, iter1, arr);
	
	/* Tests after insertion to the linked list */
	TestSListInsertBefore(linked_list, iter1, arr);
	TestSListGetHead(linked_list, iter1, arr);
	TestSListGetEnd(linked_list, iter1, arr);
	TestSListGetNext(linked_list, iter1, arr);
	
	/* removing and inserting back */
	TestSListRemove(linked_list);
	iter1 = SListGetHead(linked_list);
	SListInsertBefore(iter1, &arr[ARR_SIZE - 1]);
	iter2 = SListGetEnd(linked_list);
	
	TestSListIsIterEqual(iter1, iter2);
	SListForEach(iter1, iter2, multi_f_ptr);
	TestSListForEach(iter1, iter2, arr);
	SListDestroy(linked_list);

	return (0);
}

static void PrintTestResult(int status)
{
	if (SUCCESS == status)
	{
		printf("SUCCESS\n");
	}
	else
	{
		printf("FAILURE\n");
	}
}

static void TestSListCreate(slist_t *list)
{
	printf("List Create Test:\t\t");
	
	(NULL != list)?	PrintTestResult(SUCCESS): PrintTestResult(FAILURE);
}

static void TestIsListEmpty(slist_t *list, size_t num_of_elements)
{
	int status = SUCCESS;
	
	printf("List IsEmpty Test:\t\t");
	if (SListIsEmpty(list) && 0 == num_of_elements)
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestSListGetSize(slist_t *list, size_t num_of_elements)
{
	int status = SUCCESS;
	
	printf("List GetSize Test:\t\t");
	if (SListGetSize(list) == num_of_elements)
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestSListGetHead(slist_t *list, slist_iter_t *iter, int *arr)
{
	int status = SUCCESS;
	void *data = NULL;
	
	iter = SListGetHead(list);
	data = SListGetData(iter);
	
	printf("List GetHead and GetData Test:\t");
	if (SListIsEmpty(list) && data == NULL)
	{
		status = SUCCESS;
	}
	else if (ARR_SIZE == SListGetSize(list) && *(int *)data == arr[9])
	{
		status = SUCCESS;
	}
	else 
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void  TestSListGetEnd(slist_t *list, slist_iter_t *iter, int *arr)
{
	int status = SUCCESS;
	void *data = NULL;
	
	iter = SListGetEnd(list);
	data = SListGetData(iter);
	
	printf("List GetEnd and GetData Test:\t");
	if ((SListIsEmpty(list) && NULL == data) || *(int *)data == arr[0])
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void  TestSListGetNext(slist_t *list, slist_iter_t *iter, int *arr)
{
	int status = SUCCESS;
	void *data = NULL;
	
	iter = SListGetHead(list);
	iter = SListGetNext(iter);
	data = SListGetData(iter);
	
	printf("List GetNext and GetData Test:\t");
	if (*(int *)data == arr[ARR_SIZE - 2])
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestSListInsertBefore(slist_t *list, slist_iter_t *iter, int *arr)
{
	int status = SUCCESS;
	size_t runner = 0;
	
	printf("List InsertBefore Test:\t\t");
	iter = SListGetHead(list);
	
	for (; runner < ARR_SIZE; ++runner)
	{
		status = SListInsertBefore(iter, &arr[runner]);
	}
	
	PrintTestResult(status);
}

static void TestSListRemove(slist_t *list)
{
	int status = SUCCESS;
	slist_iter_t *iter;
	
	iter = SListGetHead(list);
	
	printf("List Remove Test:\t\t");
	SListRemove(iter);
	
	if (ARR_SIZE - 1 == SListGetSize(list))
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestSListIsIterEqual(slist_iter_t *iter1, slist_iter_t *iter2)
{
	int is_equal = SListIsIterEqual(iter1, iter2);
	int status = SUCCESS;
	
	printf("List AreItersEqual:\t\t");
	
	if (0 == is_equal)
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestSListForEach(slist_iter_t *iter_from, 
							slist_iter_t *iter_to, int *arr)
{
	int status = SUCCESS;
	size_t arr_runner = ARR_SIZE;
	slist_iter_t *iter_runner = iter_from;
	
	iter_to = SListGetNext(iter_to);
	printf("List ForEach Test:\t\t");
	
	for (; !SListIsIterEqual(iter_runner, iter_to) && 
			arr_runner > 0 && SUCCESS == status; 
			iter_runner = SListGetNext(iter_runner), --arr_runner)
	{
		void *data = SListGetData(iter_runner);
		
		if (arr[arr_runner - 1] != *(int *)data)
		{
			status = FAILURE;
		}
	}
	
	PrintTestResult(status);
}


int PrintIterData(void *data)
{
	printf("%d\n", *(int *)data);
	
	return SUCCESS;
}

int MultiplyIterData(void *data)
{
	*(int *)data *= 10;
	
	return SUCCESS;
}

int CompareIters(void *data1, void *data2)
{
	assert(NULL != data1);
	assert(NULL != data2);
	
	return (*(int *)data1 == *(int *)data2);
}


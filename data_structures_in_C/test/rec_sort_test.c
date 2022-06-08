/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/

#include <stdio.h>	/* printf */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* rand() */

#include "rec_sort.h"

#define UNUSED(x) (void)(x)
#define ARR_SIZE 15

int Cmp(const void *lhs, const void *rhs);
static void InitArr(int *arr);
static void TestMergeSort(int *arr);
static void TestBinarySearch(int *arr);
static void TestQSort(int *arr);

int main(void)
{
	int arr[ARR_SIZE] = {0};

	InitArr(arr);
	TestMergeSort(arr);
	TestBinarySearch(arr);
	InitArr(arr);
	TestQSort(arr);
	
	return (0);
}

static void InitArr(int *arr)
{
	int i = 0;

	for (; i < ARR_SIZE - 1; ++i)
	{
		arr[i] = rand() % 1000;
	}

	arr[i] = 123; /* known value for binary search */
}

int Cmp(const void *lhs, const void *rhs)
{
	assert(NULL != lhs);
	assert(NULL != rhs);

	return (*(int *)lhs - *(int *)rhs);
}

static void TestMergeSort(int *arr)
{
	int i = 0;

	MergeSort(arr, ARR_SIZE);
	for (; i < ARR_SIZE - 1 ; ++i)
	{
		assert(arr[i] <= arr[i + 1]);
	}
	printf("MergeSort Passed\n");
}

static void TestBinarySearch(int *arr)
{
	int to_find = 123;
	int not_in_arr = 9999;
	int index = -1;

	index = IterBinarySearch(arr, ARR_SIZE, to_find);
	assert(arr[index] == to_find);

	index = IterBinarySearch(arr, ARR_SIZE, not_in_arr);
	assert(-1 == index);

	index = IterBinarySearch(arr, ARR_SIZE, arr[0]);
	assert(arr[0] == arr[index]);

	index = IterBinarySearch(arr, ARR_SIZE, arr[ARR_SIZE - 1]);
	assert(arr[ARR_SIZE - 1] == arr[index]);

	printf("IterBinarySearchPassed\n");

	index = RecBinarySearch(arr, ARR_SIZE, to_find);
	assert(arr[index] == to_find);

	index = IterBinarySearch(arr, ARR_SIZE, not_in_arr);
	assert(-1 == index);
	
	printf("RecBinarySearchPassed\n");
}

static void TestQSort(int *arr)
{
	int i = 0;

	QSort(arr, ARR_SIZE, sizeof(int), &Cmp);
	for (; i < ARR_SIZE - 1 ; ++i)
	{
		assert(arr[i] <= arr[i + 1]);
	}
	printf("QuickSort Passed\n");
}
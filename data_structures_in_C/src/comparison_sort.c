/**************************************************************
* Author: Gal Dagan
* Date: 25.08.21
* Reviewer: Ido Sapan
*
* Description: Implementation of comparison sorting algorithms
*
* InfintyLabs OL108
**************************************************************/

#include <assert.h> /* assert()	*/

#include "comparison_sort.h"

/******************	Static function decleration *************************/
static void SwapTwoIntsInArr(int *num_1, int *num_2);
static int *FindMinNumInArr(int *start, int *end);
static void InsertInRightPos(int *arr_start, int *number);

/************************************************************************/

void BubbleSort(int *arr, size_t size)
{
	int swapped_flag = 1;
	int *stop_point = NULL;
	int *sorter = NULL;
		
	assert(NULL != arr);
	
	stop_point = arr + size - 1;
	
	for (; stop_point != arr && 0 != swapped_flag; --stop_point)
	{
		swapped_flag = 0;
		for (sorter = arr; sorter != stop_point; ++sorter)
		{
			if (*sorter > *(sorter + 1))
			{
				SwapTwoIntsInArr(sorter, sorter + 1);
				swapped_flag = 1;
			}
		}
	}
}

void InsertionSort(int *arr, size_t size)
{
	int *stop_point = NULL;
	int *runner = NULL;
	
	assert(NULL != arr);
	
	stop_point = arr + size;
	runner = arr + 1;
	
	for (; runner != stop_point; ++runner)
	{
		if (*runner < *(runner - 1))
		{
			InsertInRightPos(arr, runner);
		}
	}
}

void SelectionSort(int *arr, size_t size)
{
	int *stop_point = NULL;
	int *runner = NULL;
	
	assert(NULL != arr);
	
	stop_point = arr + size;
	runner = arr;
	
	for (; runner != stop_point; ++runner)
	{
		int *min_num = FindMinNumInArr(runner, stop_point);
		SwapTwoIntsInArr(min_num, runner);
	}
}


static void SwapTwoIntsInArr(int *num_1, int *num_2)
{
	int temp = *num_1;
	*num_1 = *num_2;
	*num_2 = temp;
}

static int *FindMinNumInArr(int *start, int *end)
{
	int *min_num = NULL;
	
	assert(NULL !=start);
	assert(NULL != end);
	
	min_num = start;
	for (; start != end; ++start)
	{
		if (*start < *min_num)
		{
			min_num = start;
		}
	}
	
	return min_num;
}

static void InsertInRightPos(int *arr_start, int *number)
{
	for (; number != arr_start && *number < *(number - 1); --number)
	{
		SwapTwoIntsInArr(number, number - 1);
	}
}


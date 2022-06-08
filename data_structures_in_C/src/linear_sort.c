/**************************************************************
* Author: Gal Dagan
* Date: 29.08.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Counting Sort and Radix Sort algotithms
*
* InfintyLabs OL108
**************************************************************/
#include <assert.h>	/* assert()	*/
#include <stdlib.h> /* calloc(), free() */
#include <string.h> /* memcpy() */

#include "linear_sort.h"

#define RADIX_HIST_SIZE 10

enum status
{
	SUCCESS,
	FAILURE
};

/*************************** Static function declaration ********************/
static int CountingSortWithExp(int *arr, size_t size, int *hist, 
								size_t hist_size, int exp);
static int FindMaxInArr(int *arr, size_t size);
static int *MakeHist(int exp, int max_num);
static void InitHist(int *arr, size_t size, int *hist, int exp);
static void AccumulateHist(int *hist, size_t size);
static void SortArrWithHist(int *arr, size_t size, int *hist, int *arr_copy, 
							int exp);

/*****************************************************************************/

int CountingSort(int *arr, size_t size)
{
	int *hist = NULL;
	int status = SUCCESS;
	int max_num = 0;
	
	assert(NULL != arr);
	
	max_num = FindMaxInArr(arr, size);
	hist = MakeHist(0, max_num);
	if (NULL == hist)
	{
		return FAILURE;
	}
	
	status = CountingSortWithExp(arr, size, hist, max_num + 1, 0);
	free(hist); 
	hist = NULL;
	
	return status;
}

int RadixSort(int *arr, size_t size)
{
	int *hist = NULL;
	int status = SUCCESS;
	int max_num = 0;
	int exp = 1;
	
	max_num = FindMaxInArr(arr, size);
	
	for (; FAILURE != status && (0 != (max_num / exp)) ; exp *= 10)
	{	
		hist = MakeHist(exp, max_num);
		if (NULL == hist)
		{
			return FAILURE;
		}
		status = CountingSortWithExp(arr, size, hist, RADIX_HIST_SIZE, exp);
		free(hist); 
		hist = NULL;
	}
	
	return status;
}

static int CountingSortWithExp(int *arr, size_t size, int *hist, 
								size_t hist_size, int exp)
{
	int *arr_copy = NULL; 
	
	assert(NULL != arr);
	
	arr_copy = (int *)calloc(size, sizeof(int));	
	if (NULL == arr_copy)
	{
		return FAILURE;
	}
	
	InitHist(arr, size, hist, exp);
	AccumulateHist(hist, hist_size);
	SortArrWithHist(arr, size, hist, arr_copy, exp);
	free(arr_copy); 
	arr_copy = NULL;
	
	return SUCCESS;
}

static int FindMaxInArr(int *arr, size_t size)
{
	size_t runner = 0;
	int max_num = *arr;
	
	for (; runner < size; ++runner)
	{
		if (arr[runner] > max_num)
		{
			max_num = arr[runner];
		}
	}
	
	return max_num;
}

static int *MakeHist(int exp, int max_num)
{
	int *hist = NULL;
	
	if (0 == exp)
	{
		hist = (int *)calloc(max_num + 1, sizeof(int));
	}
	else
	{
		hist = (int *)calloc(RADIX_HIST_SIZE, sizeof(int));
	}
	
	if (NULL == hist)
	{
		return NULL;
	}
	
	return hist;
}

static void InitHist(int *arr, size_t size, int *hist, int exp)
{
	size_t runner = 0;
	
	if (0 == exp)
	{
		for (; runner < size; ++runner)
		{
			hist[arr[runner]] += 1;
		}
	}
	else
	{
		for (; runner < size; ++runner)
		{
			hist[(arr[runner] / exp) % 10] += 1;
		}
	}
}

static void AccumulateHist(int *hist, size_t size)
{
	size_t runner = 1;
	
	for (; runner < size; ++runner)
	{
		hist[runner] += hist[runner - 1];
	}
}

static void SortArrWithHist(int *arr, size_t size, int *hist, int *arr_copy, 
							int exp)
{
	int *runner = arr + size - 1;
	
	if (0 == exp)
	{
		for (; runner >= arr; --runner)
		{
			hist[*runner] -= 1;
			arr_copy[hist[*runner]] = *runner;
		}
	}
	else
	{
		for (; runner >= arr; --runner)
		{
			hist[(*runner / exp) %  10] -= 1;
			arr_copy[hist[(*runner / exp) %  10]] = *runner;
		}
	}
	memcpy(arr, arr_copy, sizeof(int) * size);
}



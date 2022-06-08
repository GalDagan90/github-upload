/**************************************************************
* Author: Gal Dagan	
* Date: 29.08.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Counting Sort and Radix Sort algotithms
*
* InfintyLabs OL108
**************************************************************/

#include <stdio.h> 	/* printf()	*/
#include <time.h>	/* clock_t, clock() */
#include <stdlib.h>	/* rand()	*/
#include <assert.h>	/* assert()	*/

#include "linear_sort.h"

#define ARR_SIZE 5000

typedef enum algorithm
{
	COUNTING,
	RADIX
}algo;

static void InitArr(int *arr, algo id);
static void TestAlgo(int *arr, algo id);

int main(void)
{
	int arr[ARR_SIZE] = {0};
	
	InitArr(arr, COUNTING);
	TestAlgo(arr, COUNTING);
	InitArr(arr, RADIX);
	TestAlgo(arr, RADIX);
	
	return (0);
}

static void InitArr(int *arr, algo id)
{
	size_t runner = 0;
	
	if (COUNTING == id)
	{
		
		for (; runner < ARR_SIZE; ++runner)
		{
			arr[runner] = rand() % 100;
		}
	}
	else
	{
		for (; runner < ARR_SIZE; ++runner)
		{
			arr[runner] = rand() % 1000000;
		}
	}

}

static void TestAlgo(int *arr, algo id)
{
	clock_t start = 0, end = 0;
	size_t i = 0;
	
	if (COUNTING == id)
	{

		start = clock();
		CountingSort(arr, ARR_SIZE);
		end = clock();
	}
	else
	{
		start = clock();
		RadixSort(arr, ARR_SIZE);
		end = clock();
	}
	
	for (; i < ARR_SIZE - 1; ++i)
	{
		/*printf("%d\n", arr[i]);*/
		assert(arr[i] <= arr[i + 1]);
	}
	printf("Algo %d Total time: %ld\n", id, end - start);
}

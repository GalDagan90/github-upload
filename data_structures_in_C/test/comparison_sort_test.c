#include <stdio.h> 	/* printf()	*/
#include <time.h>	/* clock_t, clock() */
#include <stdlib.h>	/* rand()	*/
#include <assert.h>	/* assert()	*/

#include "comparison_sort.h"

#define ARR_SIZE 5000

typedef enum algorithm
{
	BUBBLE,
	INSERTION,
	SELECTION
}algo;

static void InitArr(int *arr);
static void TestAlgo(int *arr, algo id);

int main(void)
{
	int arr[ARR_SIZE] = {0};
	
	InitArr(arr);
	TestAlgo(arr, BUBBLE);
	InitArr(arr);
	TestAlgo(arr, INSERTION);
	InitArr(arr);
	TestAlgo(arr, SELECTION);
	
	return (0);
}

static void InitArr(int *arr)
{
	size_t i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		arr[i] = rand();
	}
}

static void TestAlgo(int *arr, algo id)
{
	clock_t start = 0, end = 0;
	size_t i = 0;
	
	switch (id)
	{
		case BUBBLE:
			start = clock();
			BubbleSort(arr, ARR_SIZE);
			end = clock();
			break;		
		case INSERTION:
			start = clock();
			InsertionSort(arr, ARR_SIZE);
			end = clock();
			break;		
		case SELECTION:
			start = clock();
			SelectionSort(arr, ARR_SIZE);
			end = clock();
			break;		
		default:
			break;
	}
	
	for (; i < ARR_SIZE - 1 ; ++i)
	{
		assert(arr[i] < arr[i + 1]);
	}
	printf("Algo %d Total time: %ld\n", id, end - start);
}

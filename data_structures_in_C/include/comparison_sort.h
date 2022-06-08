/**************************************************************
* Author: Gal Dagan
* Date: 25.08.21 
* Reviewer: Ido Sapan
*
* Description: Implementation of comparison sorting algorithms
*
* InfintyLabs OL108
**************************************************************/

#ifndef __COMP_SORT_H_OL108_ILRD__
#define __COMP_SORT_H_OL108_ILRD__

#include <stddef.h>	/* size_t	*/

/**
 * @Description: Sorting the array using bubble sort algorithm.
 * @Parameters: arr - pointer to an array of integers.
 *				size - array size.
 * @Return: void.
 * @Complexity: O(n^2)
**/
void BubbleSort(int *arr, size_t size);

/**
 * @Description: Sorting the array using insertion sort algorithm.
 * @Parameters: arr - pointer to an array of integers.
 *				size - array size.
 * @Return: void.
 * @Complexity: O(n^2)
**/
void InsertionSort(int *arr, size_t size);

/**
 * @Description: Sorting the array using selection sort algorithm.
 * @Parameters: arr - pointer to an array of integers.
 *				size - array size. 
 * @Return: void.
 * @Complexity: O(n^2)
**/
void SelectionSort(int *arr, size_t size);

#endif /* __COMP_SORT_H_OL108_ILRD__ */


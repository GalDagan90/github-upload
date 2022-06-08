/**********************************************************************
* Author: Gal Dagan	
* Date: 29.08.21
* Reviewer: Aharon Luzon
*
* Description: Defenitions of Counting Sort and Radix Sort algotithms
*
* InfintyLabs OL108
**********************************************************************/
#ifndef __LINEAR_SORT_H_OL108_ILRD__
#define __LINEAR_SORT_H_OL108_ILRD__

#include <stddef.h>	/* size_t	*/

/**
 * @Description: Sorting the array using Counting Sort algorithm.
 * @Parameters: arr - pointer to an array of integers.
 *				size - array size.
 * @Return: 0 if sorting was succesfull, non_zero otherwise.
 * @note n- arr length, r- max_num - min_num in arr.
 * @Complexity: O(n+r)
**/
int CountingSort(int *arr, size_t size);

/**
 * @Description: Sorting the array using Radix Sort algorithm.
 * @Parameters: arr - pointer to an array of integers.
 *				size - array size.
 * @Return: 0 if sorting was succesfull, non_zero otherwise.
 * @note k- maximum number in the arr, d- num of digits in k.
 * @Complexity: O(n * (k/d))
**/
int RadixSort(int *arr, size_t size);

#endif /* __LINEAR_SORT_H_OL108_ILRD__ */

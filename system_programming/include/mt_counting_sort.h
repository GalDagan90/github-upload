/****************************************************************************
*Author : Gal Dagan
*Date : 30.10.21	
*Reviewer :	Ido Sapan
*
*Description : Multi-threading counting sort.
*
*Lab : RD5678
****************************************************************************/
#ifndef __MT_COUNTING_SORT_H_RD5678_ILRD__
#define __MT_COUNTING_SORT_H_RD5678_ILRD__

#include <stddef.h> /* size_t */

/**
 * @Description: Sorting a given array using Counting Sort algorithm and
 * 				multiple threads.
 * @Parameters: arr - pointer to an array of chars.
 *				size - array size.
 *				thread_num - number of threads.
 * @Return: 0 if sorting was succesfull, non_zero otherwise.
 * @Complexity: O(n+r)
 * @note: n- arr length, r- max_char - min_char in arr.
 * 		0 thread_num is not allowed.
**/
int MTCountingSort(char *arr, size_t size, size_t thread_num);

#endif /* __MT_COUNTING_SORT_H_RD5678_ILRD__ */

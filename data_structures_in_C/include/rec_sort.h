/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __REC_SORT_H_OL108_ILRD__
#define __REC_SORT_H_OL108_ILRD__

/**
 * @Description: returns index of the found element. if the element
 * 				was not found returns -1.
 * @Parameters: arr_to_sort - pointer to sorted array of integers.
 *				num_elements - array size.
 *				to_find - an integer to look for in the array.
 * @Return: the index of the element if found. -1 otherwise.
 * @Complexity: O(log(n))
**/
int IterBinarySearch(int *arr_to_sort, int num_elements, int to_find);

/**
 * @Description: returns index of the found element. if the element
 * 				was not found returns -1.
 * @Parameters: arr_to_sort - pointer to sorted array of integers.
 *				num_elements - array size.
 *				to_find - an integer to look for in the array.
 * @Return: the index of the element if found. -1 otherwise.
 * @Complexity: O(log(n))
**/
int RecBinarySearch(int *arr_to_sort, int num_elements, int to_find);

/**
 * @Description: Sorting the array using merge sort algorithm.
 * @Parameters: arr_to_sort - pointer to array of integers to be sorted.
 *				num_elements - array size.
 * @Return: 0 if sorting was succeful. Non-zero otherwise.
 * @Complexity: O(log(n))
**/
int MergeSort(int *arr_to_sort, size_t num_elements);

/**
 * @Description: Sorting the array using quick sort algorithm.
 * @Parameters: base - pointer to an array to be sorted.
 *				nmemb - array size.
 *				size - size (in bytes) of each member in the array.
 *				compar - pointer to a compare function.
 * @Return: void.
 * @Complexity: average - O(log(n)). Worst - O(n^2).
**/
void QSort(void *base, size_t nmemb, size_t size,
		   int (*compar)(const void *, const void *));


#endif /* __REC_SORT_H_OL108_ILRD__ */

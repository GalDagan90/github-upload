/****************************************************************************
*Author : Gal Dagan
*Date : 15.09.21    
*Reviewer : Keren Robbins
*
*Description : Implementation of Binary Search, Merge Sort and Quick Sort
*
*Lab : ol108
****************************************************************************/

#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memcpy() */
#include <assert.h> /* assert */

#include "rec_sort.h"

/***************************************************************************
*                         Static Function Decleration                      *
***************************************************************************/

static void InternalMergeSort(int *arr, int size, int *helper_arr);
static void MergeSortedArrays(int *arr1, int size_1, int *arr2, int size_2, 
                                int *merged_arr);
static int InternalRecBinarySearch(int *arr, int left, int right, int to_find);
static void SwapTwoElements(void *x, void *y, size_t size);
static void PivotPartition(void *base, size_t nmemb, size_t size,
                int (*compar)(const void *, const void *), void **pivot);                    
/**************************************************************************/
int IterBinarySearch(int *arr_to_sort, int num_elements, int to_find)
{
    int left = 0;
    int right = num_elements - 1;

    assert(NULL != arr_to_sort);

    while (left <= right)
    {
        int mid = (left + right) / 2;
        if (arr_to_sort[mid] == to_find)
        {
            return mid;
        }
        if (arr_to_sort[mid] < to_find)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return (-1);
}

int RecBinarySearch(int *arr_to_sort, int num_elements, int to_find)
{
    assert(NULL !=arr_to_sort);
    
    return (InternalRecBinarySearch(arr_to_sort, 0, num_elements - 1, to_find));
}

int MergeSort(int *arr_to_sort, size_t num_elements)
{
    int *temp_arr = NULL;

    assert(NULL != arr_to_sort);

    temp_arr = (int *)malloc(sizeof(int) * num_elements);
    if (NULL == temp_arr)
    {
        return (-1);
    }

    InternalMergeSort(arr_to_sort, num_elements, temp_arr);
    free(temp_arr);
    
    return (0);
}

void QSort(void *base, size_t nmemb, size_t size,
		   int (*compar)(const void *, const void *))
{
    void *pivot = NULL;
    size_t left_nmemb = 0;
    assert(NULL != base);

    if (nmemb < 2)
    {
        return;
    }

    pivot = base;
    SwapTwoElements(pivot, (char *)pivot + (nmemb / 2) * size, size);
    PivotPartition(base, nmemb, size, compar, &pivot);
    left_nmemb = ((char *)pivot - (char *)base) / size;
    QSort(base, left_nmemb, size, compar);
    QSort((char *)pivot + size, nmemb - left_nmemb - 1, size, compar);
}           
/****************************************************************************/
static void InternalMergeSort(int *arr, int size, int *helper_arr)
{
    int mid = size / 2;

    if (size < 2)
    {
        return;
    }

    InternalMergeSort(arr, mid, helper_arr);
    InternalMergeSort(arr + mid, size - mid, helper_arr);
    MergeSortedArrays(arr, mid, arr + mid, size - mid, helper_arr);
    memcpy(arr, helper_arr, sizeof(int) * size);
}

static void MergeSortedArrays(int *arr1, int size_1, int *arr2, int size_2, 
                                int *merged_arr)
{
    int runner_1 = 0;
    int runner_2 = 0;
    int merge_runner = 0;

    for (; (runner_1 < size_1) && (runner_2 < size_2); ++merge_runner)
    {
        if (arr1[runner_1] <= arr2[runner_2])
        {
            merged_arr[merge_runner] = arr1[runner_1];
            ++runner_1;
        }
        else
        {
            merged_arr[merge_runner] = arr2[runner_2];
            ++runner_2;
        }
    }

    for (; runner_1 < size_1; ++runner_1, ++merge_runner)
    {
        merged_arr[merge_runner] = arr1[runner_1];
    }
    for (; runner_2 < size_2; ++runner_2, ++merge_runner)
    {
        merged_arr[merge_runner] = arr2[runner_2];
    }
}        

static int InternalRecBinarySearch(int *arr, int left, int right, int to_find)
{
    int mid = (left + right) / 2;

    if (left > right)
    {
        return (-1);
    }
    if (arr[mid] < to_find)
    {
        return InternalRecBinarySearch(arr, mid + 1, right, to_find);
    }
    else if (arr[mid] > to_find)
    {
        return InternalRecBinarySearch(arr, left, mid - 1, to_find);
    }
    else
    {
        return mid;
    }
}

static void PivotPartition(void *base, size_t nmemb, size_t size,
                int (*compar)(const void *, const void *), void **pivot)
{
    void *left = ((char *)base + size);
    void *right = ((char *)base + (size * nmemb));

    while(left <= right)
    {
        for (; left <= right && compar(left, *pivot) <= 0;
                left = (char *)left + size)
        {
            /* empty loop */
        }
        for (; right >= left && compar(right, *pivot) > 0;
                right = (char *)right - size)
        {
            /* empty loop */
        }
        if (left < right)
        {
            SwapTwoElements(left, right, size);
        }
    }
    SwapTwoElements(*pivot, right, size);
    *pivot = right;
}                

static void SwapTwoElements(void *x, void *y, size_t size)
{
    char temp = 0;
    size_t i = 0;

    for (; i < size; ++i)
    {
        temp = ((char *)x)[i];
        ((char *)x)[i] = ((char *)y)[i];
        ((char *)y)[i] = temp; 
    }
}
/******************************************************************************
 * Author: Gal Dagan                                                          
 * Reviewer: Meital Kozhidov                                                                
 * Created: 10.08.2021                                                        
 *                                                                            
 * Description: Implementatsorted of sorted list list                                        
 *                                                                            
 *                                                                            
 * Infinity Labs OL108                                                        
 *****************************************************************************/

#ifndef __SORTED_LIST_H_OL108_ILRD__
#define __SORTED_LIST_H_OL108_ILRD__

#include "doubly_linked_list.h"

typedef struct sorted_list sort_list_t;

typedef struct sort_list_iter
{
	dlist_iter_t internal;
} sort_list_iter_t;

/**
 * @Description: Function pointer type that compares the data of two iterators.
 * @Parameters: Two void pointers to the data from two iterators.
 * @Return: Zero if they are equal, a negative number if lhs should be before
 *          rhs, and a positive number if rhs should be before lhs.
**/
typedef int (* sort_cmp_func_t)(const void *, const void *);

/******************************************************************************
 *                           FUNCTION DECLARATIONS                            *
 ******************************************************************************/

/**
 * @Description: Create an empty sorted list with cmp_func.
 * @Parameters: cmp_func.
 * @Return: A pointer to the new list.
 * @Complexity: O(1).
**/
sort_list_t *SortListCreate(sort_cmp_func_t cmp_func);

/**
 * @Description: Destroy the sorted list (but not the data).
 * @Parameters: list - a pointer to the sorted list.
 * @Return: void.
 * @Complexity: O(n).
**/
void SortListDestroy(sort_list_t *list);

/**
 * @Description: Get data from an iterator.
 * @Parameters: iter - the iterator.
 * @Return: A void pointer to the data.
 * @Complexity: O(1).
**/
void *SortListGetData(sort_list_iter_t iter);

/**
 * @Description: Get the next iterator.
 * @Parameters: iter - the iterator.
 * @Return: The next iterator.
 * @Complexity: O(1).
**/
sort_list_iter_t SortListGetNext(sort_list_iter_t iter);

/**
 * @Description: Get the previous iterator.
 * @Parameters: iter - the iterator.
 * @Return: The previous iterator.
 * @note: If the iter is head iterator,
 * the function will return the head iter.
 * @Complexity: O(1).
**/
sort_list_iter_t SortListGetPrev(sort_list_iter_t iter);

/**
 * @Description: Get the head iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: The fitst valid iterator in the list.
 * @Complexity: O(1).
**/
sort_list_iter_t SortListGetBegin(const sort_list_t *list);

/**
 * @Description: Get the dummy tail iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: The dummy tail iterator.
 * @Complexity: O(1).
**/
sort_list_iter_t SortListGetEnd(const sort_list_t *list);

/**
 * @Description: Insert data to the list according to the cmp_func.
 * @Parameters: list - the iterator.
 *              data - a void pointer to to the new data.
 * @Return: The new iterator if the insertion was successful, otherwise 
 *			returns the dummy tail.
 * @Notes: The data is copied by reference - only the address is stored.
 * @Complexity: O(n).
**/
sort_list_iter_t SortListInsert(sort_list_t *list, const void *data);

/**
 * @Description: Remove a given iterator from the list.
 * @Parameters: iter - the iterator.
 * @Return: The next iterator.
 * @Notes: If iter is the dummy, the function does nothing.
 * @Complexity: O(1).
**/
sort_list_iter_t SortListRemove(sort_list_iter_t iter);

/**
 * @Description: Removes the iterator before the dummy tail from the list.
 * @Parameters: list - a pointer to the list.
 * @Return: The dummy tail iterator.
 * @Complexity: O(1).
**/
void *SortListPopBegin(sort_list_t *list);

/**
 * @Description: Removes the head iterator from the list.
 * @Parameters: list - a pointer to the list.
 * @Return: The new head iterator.
 * @Complexity: O(1).
**/
void *SortListPopEnd(sort_list_t *list);

/**
 * @Description: Get the size of the sorted list.
 * @Parameters: list - a pointer to the list.
 * @Return: The size of the list (number of elements in the list).
 * @Complexity: O(n).
**/
size_t SortListSize(const sort_list_t *list);

/**
 * @Description: Check if the list is empty.
 * @Parameters: list - a pointer to the list.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
int SortListIsEmpty(const sort_list_t *list);

/**
 * @Description: Compare two iterators to check if they are identical.
 * @Parameters: iter1 - the first iterator.
 *              iter2 - the second iterator.
 * @Return: One if they are identical, zero if not.
 * @Complexity: O(1).
**/
int SortListIsSameIter(sort_list_iter_t iter1, sort_list_iter_t iter2);

/**
 * @Description: Run an operation on each iterator in a section of the list.
 * @Parameters: from - the first iterator in the section.
 *              to - the first iterator after the section (the operation will 
 *              not be performed on its data).
 *              operation_func - a pointer to a function to perform on the data.
 *              The function should recieve a void pointer and return zero if
 *              the operation was successful or non-zero if not.
 * @Return: Zero if all operations were successful, or the return value of
 *          operation_func if not.
 * @Notes: If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n).
**/
int SortListForEach(sort_list_iter_t from, sort_list_iter_t to
		, operation_func_t operation_func, void *param);

/**
 * @Description: Find an iterator with matching data in the list with the 
 * 				cmp_func of the list.
 * @Parameters: list - the list to look for the elemet.
 *              to_find - a pointer to the data we search for.
 * @Return: An iterator with the same value, or the dummy tail if no match was 
 *          found.
 * @Notes: If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n).
**/
sort_list_iter_t SortListFind(const sort_list_t *list, const void *to_find);

/**
 * @Description: Find an iterator with matching data in a section of the list 
 * 				with another cmp_func.
 * @Parameters: from - the first iterator in the section.
 *              to - the first iterator after the section (not included in the
 *              search).
 *              to_find - a pointer to the data we search for.
 *              is_match - a pointer to a function to compare the data. The
 *              function should recieve two void pointers and return one if
 *              their data is equal or zero if not.
 * @Return: An iterator with the same value, or the dummy tail if no match was 
 *          found.
 * @Notes: If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n).
**/
sort_list_iter_t SortListFindIf(sort_list_iter_t from, sort_list_iter_t to
		, const void *to_find, is_match_t is_match);

/**
 * @Description: merges the src list into the dest list 
 *				(src will become an empty list).
 * @Parameters: dest - the list to marge to.
 *              src - the list to marge from.
 * @Return: dest after merge
 * @Notes: If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n + m).
**/
sort_list_t *SortListMerge(sort_list_t *dest, sort_list_t *src);

#endif /* __SORTED_LIST_H_OL108_ILRD__ */

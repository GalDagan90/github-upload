/**************************************************************
* Author: Gal Dagan
* Date: 09.08.21
* Reviewer: Eyal Dafni
*
* Description: Implementation of Doubly linked list data structure
*
* InfintyLabs OL108
**************************************************************/

#ifndef __DOUBLY_LINKED_LIST_H_OL108_ILRD__
#define __DOUBLY_LINKED_LIST_H_OL108_ILRD__

#include <stddef.h> /* size_t */

/******************** typedef ****************************/
typedef struct dlist_node dlist_node_t;
typedef struct dlist dlist_t;

/**********************************************************/

/**************** Struct Declaration **********************/

typedef struct dlist_iter
{
	dlist_node_t *internal;
}dlist_iter_t;

/***********************************************************/

/**
 * @Description: Pointer to a function that compares the data of an iterator
 *               with data in another void pointer.
 * @Parameters: Two void pointers to the data to compare.
 * @Return: One if they are equal, zero if not.
**/
typedef int (*is_match_t)(const void *, const void *);

/**
 * @Description: Pointer to a function that performes an operation on the value
 *               from an iterator.
 * @Parameters: A pointers to the data from an iterator.
 * @Return: Zero if the operation was successful, non-zero if not.
**/
typedef int (*operation_func_t)(void *, void *);

/******************************************************************************
*                           FUNCTION DECLARATIONS                            *
******************************************************************************/
/**
 * @Description: Create an empty doubly linked list (containing only dummies).
 * @Parameters: void.
 * @Return: A pointer to the new list.
 * @Complexity: O(1).
**/
dlist_t *DListCreate(void);

/**
 * @Description: Destroy the linked list (but not the data).
 * @Parameters: list - a pointer to the linked list.
 * @Return: void.
 * @Complexity: O(n).
**/
void DListDestroy(dlist_t *list);

/**
 * @Description: Get the size of the linked list.
 * @Parameters: list - a pointer to the list.
 * @Return: The size of the list (number of elements in the list).
 * @Complexity: O(n).
**/
size_t DListGetSize(const dlist_t *list);

/**
 * @Description: Check if the list is empty (contains only dummies).
 * @Parameters: list - a pointer to the list.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
int DListIsEmpty(const dlist_t *list);

/**
 * @Description: Get data from an iterator.
 * @Parameters: iter - the iterator.
 * @Return: A void pointer to the data.
 * @Complexity: O(1).
**/
void *DListGetData(dlist_iter_t iter);

/**
 * @Description: Get the next iterator.
 * @Parameters: iter - the iterator.
 * @Return: The next iterator.
 * @Complexity: O(1).
**/
dlist_iter_t DListGetNext(dlist_iter_t iter);

/**
 * @Description: Get the previous iterator.
 * @Parameters: iter - the iterator.
 * @Return: The previous iterator.
 * @Complexity: O(1).
**/
dlist_iter_t DListGetPrev(dlist_iter_t iter);

/**
 * @Description: Get the head iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: The head iterator (not the dummy head).
 * @Complexity: O(1).
**/
dlist_iter_t DListGetHead(dlist_t *list);

/**
 * @Description: Get the dummy tail iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: The dummy tail iterator.
 * @Complexity: O(1).
**/
dlist_iter_t DListGetEnd(dlist_t *list);

/**
 * @Description: Insert data to a new iteratior before a given iterator.
 * @Parameters: iter - the iterator.
 *              data - a void pointer to to the new data.
 * @Return: The new iterator if the insertion was successful, the dummy tail
 *          iterator if not.
 * @Notes: The data is copied by reference - only the address is stored.
 * @Complexity: O(1).
**/
dlist_iter_t DListInsertBefore(dlist_iter_t iter, const void *data);

/**
 * @Description: Remove a given iterator from the list.
 * @Parameters: iter - the iterator.
 * @Return: The next iterator.
 * @Notes: If iter is the dummy, the function returns it and does nothing.
 * @Complexity: O(1).
**/
dlist_iter_t DListRemove(dlist_iter_t iter);

/**
 * @Description: Insert data to a new iterator before the dummy tail iterator.
 * @Parameters: list - a pointer to the list.
 *              data - a void pointer to to the new data.
 * @Return: The new iterator if the insertion was successful, the dummy tail
 *          iterator if not.
 * @Complexity: O(1).
**/
dlist_iter_t DListPushBack(dlist_t *list, const void *data);

/**
 * @Description: Insert data to a new iterator before the head iterator (not 
 *               the dummy head).
 * @Parameters: list - a pointer to the list.
 *              data - a void pointer to to the new data.
 * @Return: The new head iterator if the insertion was successful, the dummy
 *          tail iterator if not.
 * @Complexity: O(1).
**/
dlist_iter_t DListPushFront(dlist_t *list, const void *data);

/**
 * @Description: Remove the iterator before the dummy tail from the list.
 * @Parameters: list - a pointer to the list.
 * @Return: The data of the removed iterator.
 * @Complexity: O(1).
**/
void *DListPopBack(dlist_t *list);

/**
 * @Description: Remove the head iterator (not the dummy head) from the list.
 * @Parameters: list - a pointer to the list.
 * @Return: The data of the removed iterator.
 * @Complexity: O(1).
**/
void *DListPopFront(dlist_t *list);

/**
 * @Description: Compare two iterators to check if they are identical.
 * @Parameters: iter1 - the first iterator.
 *              iter2 - the second iterator.
 * @Return: One if they are identical, zero if not.
 * @Complexity: O(1).
**/
int DListIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2);

/**
 * @Description: Run an operation on each iterator in a section of the list.
 * @Parameters: from - the first iterator in the section.
 *              to - the first iterator after the section (the operation will 
 *              not be performed on its data).
 *              operation_func - a pointer to a function to operate on the data.
 *              The function should recieve a void pointer and return zero if
 *              the operation was successful or non-zero if not.
 * @Return: Zero if all operations were successful, or the return value of
 *          operation_func if not.
 * @Notes: The function returns immediately if operation_func fails.
 *         If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n).
**/
int DListForEach(dlist_iter_t from, dlist_iter_t to
		, operation_func_t operation_func, void *param);

/**
 * @Description: Find an iterator with matching data in a section of the list.
 * @Parameters: from - the first iterator in the section.
 *              to - the first iterator after the section (not included in the
 *              search).
 *              ot_find - a pointer to the data we search for.
 *              is_match - a pointer to a function to compare the data. The
 *              function should recieve two void pointers and return one if
 *              their data is equal or zero if not.
 * @Return: An iterator with the same value, or the dummy tail if no match was 
 *          found.
 * @Notes: If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n).
**/
dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, const void *to_find
		, is_match_t is_match);

/**
 * @Description: Find all iterators with matching data in a section of the list.
 * @Parameters: from - the first iterator in the section.
 *              to - the first iterator after the section (not included in the
 *              search).
 *              ot_find - a pointer to the data we search for.
 *              is_match - a pointer to a function to compare the data. The
 *              function should recieve two void pointers and return one if
 *              their data is equal or zero if not.
 *              output_list - a pointer to a list that all iterators with
 *              matching data will be copied to. The iterators will be inserted 
 *              in a similar manner to using DListPushBack, in their order in
 *              the section.
 * @Return: Zero if copying to the new list was succesful, non-zero if not.
 * @Notes: If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(n).
**/
int DListMultiFind(dlist_iter_t from, dlist_iter_t to, const void *to_find
		, is_match_t is_match, dlist_t *output_list);

/**
 * @Description: Splice a section of a list to the back of an iterator.
 * @Parameters: from - the first iterator to be cut in the splice.
 *              to - the last iterator in section which will not be spliced.
 *              dest - an iterator in the destination. The spliced section will
 *              be inserted before dest.
 * @Return: dest.
 * @Notes: from and to will be linked to each other after the splicing.
 *         If to is not after from in the same list, the behaviour is undefined.
 * @Complexity: O(1).
**/
dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t dest);

#endif /* __DOUBLY_LINKED_LIST_H_OL108_ILRD__ */

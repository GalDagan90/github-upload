#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h> /* size_t */

/************* typedefs *******************************/
typedef struct node slist_iter_t;
typedef struct slist slist_t;

/**
 * @Description: Pointer to a function that compares the values from two
 *               iterators.
 * @Parameters: Two void pointers to the data from two iterators.
 * @Return: Zero if they are equal, non-zero if not.
**/
typedef int (* cmp_func_ptr)(void *, void *);

/**
 * @Description: Pointer to a function that performes an operation on the value
 *               from an iterator.
 * @Parameters: A pointers to the data from an iterator.
 * @Return: Zero if the action was successful, non-zero if not.
**/
typedef int (* operation_func_ptr)(void *);
/************* function declaration *******************************/

/**
 * @Description: Creates an empty singly linked list (containing only a dummy).
 * @Parameters: void.
 * @Return: A pointer to the new list.
 * @Complexity: O(1).
**/
slist_t *SListCreate(void); 

/**
 * @Description: Destroys the linked list (but not the data).
 * @Parameters: list - a pointer to the linked list.
 * @Return: void.
 * @Complexity: O(n).
**/
void SListDestroy(slist_t *list); 

/**
 * @Description: Inserts data to a new iteratior before a given iterator.
 * @Parameters: iter - a pointer to the iterator.
 *              data - a void pointer to to the new data.
 * @Return: zero if the insertion was successful, non-zero if not.
 * @Notes: The data is copied by reference - only the address is stored.
 * @Complexity: O(1).
**/
int SListInsertBefore(slist_iter_t *iter, void *data); 

/**
 * @Description: Removes a given iterator from the list.
 * @Parameters: iter - a pointer to the iterator.
 * @Return: The next iterator, or the dummy tail if removal fails.
 * @Notes: If iter is the dummy, the function returns it and does nothing.
 * @Complexity: O(1).
**/
slist_iter_t *SListRemove(slist_iter_t *iter); 

/**
 * @Description: Get data from an iterator.
 * @Parameters: iter - a pointer to the iterator.
 * @Return: A void pointer to the data.
 * @Complexity: O(1).
**/
void *SListGetData(const slist_iter_t *iter); 

/**
 * @Description: Get the size of the linked list.
 * @Parameters: list - a pointer to the list.
 * @Return: The size of the list (=number of elements in the list).
 * @Complexity: O(n).
**/
size_t SListGetSize(const slist_t *list); 

/**
 * @Description: Check if the list is empty (only consists of dummy tail).
 * @Parameters: list - a pointer to the list.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
int SListIsEmpty(const slist_t *list); 

/**
 * @Description: Compare two iterators to check if they are identical.
 * @Parameters: iter1 - a pointer to the first iterator.
 *              iter2 - a pointer to the second iterator.
 * @Return: One if they are equal, zero if not.
 * @Complexity: O(1).
**/
int SListIsIterEqual(const slist_iter_t *iter1, const slist_iter_t *iter2); 

/**
 * @Description: Finds an iter in a linked list
 * @Parameters: iter_from - a pointer to the first iterator.
 *              iter_to - a pointer to the last iterator.
 *				iter - the pointer to find
 *				cmp_func - comparison function to check equality between iters
 * @Return: the iterator if found or dummy if not
 * @Complexity: O(n).
**/
slist_iter_t *SListFind(const slist_iter_t *iter_from, const slist_iter_t *iter_to
    , const slist_iter_t *iter, cmp_func_ptr cmp_func);

/**
 * @Description: Get the next iterator.
 * @Parameters: iter - a pointer to the iterator.
 * @Return: A pointer to the next iterator.
 * @Complexity: O(1).
**/
slist_iter_t *SListGetNext(const slist_iter_t *iter); 

/**
 * @Description: Get the head iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: A pointer to the head iterator.
 * @Complexity: O(1).
**/
slist_iter_t *SListGetHead(const slist_t *list); 

/**
 * @Description: Get pointer to the last iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: A pointer to the last iterator. 
 * @Complexity: O(n).
**/
slist_iter_t *SListGetEnd(const slist_t *list); 

/**
 * @Description: Operates on the data inside a specific range
 * @Parameters: iter_from - a pointer to the first iterator.
 *              iter_to - a pointer to the last iterator.
 *				operation_func - operation function altering iters data
 * @Return: zero if operation is successful
 * @Complexity: O(n).
**/
int SListForEach(slist_iter_t *iter_from, slist_iter_t * const iter_to
    , operation_func_ptr operation_func);  

/**
 * @Description: Get the dummy tail iterator.
 * @Parameters: list - a pointer to the list.
 * @Return: A pointer to the dummy iterator.
 * @Complexity: O(1).
**/
slist_iter_t *GetDummyTailFromList(slist_t *list);

#endif /* __SLIST_H__ */

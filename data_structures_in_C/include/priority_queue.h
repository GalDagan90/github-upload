/**************************************************************
* Author: Gal Dagan
* Date: 12.08.21
* Reviewer: Eyal Dafni
*
* Description: Implementation of Priority queue
*
* InfintyLabs OL108
**************************************************************/

#ifndef __PRIORITY_QUEUE_H_OL108__
#define __PRIORITY_QUEUE_H_OL108__

#include <stddef.h> /* size_t */

/***************** typedef declaration *******************************/
typedef struct pq pq_t;

/**
 * @Description: Function pointer type that compares priority of data.
 * @Parameters: Two void pointers to the data from two iterators.
 * @Return: Zero if they are equal, a negative number if lhs has a lower 
 * 			priority than rhs, and a positive number if lhs has a higher
 * 			priority than rhs.
**/
typedef int (*pq_cmp_priority_t)(const void *lhs, const void *rhs);

/**
 * @Description: Pointer to a function that compares the data of an iterator
 *               with data in another void pointer.
 * @Parameters: Two void pointers to the data to compare.
 * @Return: One if they are equal, zero if not.
**/
typedef int (*is_match_pq_t)(const void *lhs, const void *rhs);


/******************************************************************************
 *                           FUNCTION DECLARATIONS                            *
 ******************************************************************************/

/**
 * @Description: Create an empty priority queue with assosiated cmp_func.
 * @Parameters: cmp_func.
 * @Return: A pointer to the new priority queue.
 * @Complexity: O(1).
**/
pq_t *PQCreate(pq_cmp_priority_t cmp_priority);

/**
 * @Description: Destroy the priority queue (but not the data).
 * @Parameters: pqueue - a pointer to the priority queue.
 * @Return: void.
 * @Complexity: O(n).
**/
void PQDestroy(pq_t *pqueue);

/**
 * @Description: Insert data to the queue according to its priority.
 * @Parameters: pqueue - a pointer to the priority queue.
 *              data - a void pointer to to the new data.
 * @Return: 0 if the insertion was successful, otherwise returns 1.
 * @Notes: The data is copied by reference - only the address is stored.
 * @Complexity: O(n).
**/
int PQEnqueue(pq_t *pqueue, const void *data);

/**
 * @Description: Removes the frontmost element in the priority queue.
 * @Parameters: pqueue - a pointer to the priority queue.
 * @Return: A pointer to the data of the removed element.
 * @Complexity: O(1).
**/
void *PQDequeue(pq_t *pqueue);

/**
 * @Description: Return the data of frontmost element in the priority queue.
 * @Parameters: pqueue - a pointer to the queue.
 * @Return: A void pointer to the data of the element with the highest priority.
 * @Complexity: O(1).
**/
void *PQPeek(const pq_t *pqueue);

/**
 * @Description: Check if the priority queue is empty.
 * @Parameters: pqueue - a pointer to the queue.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
int PQIsEmpty(const pq_t *pqueue);

/**
 * @Description: Get the size of the priority queue.
 * @Parameters: pqueue - a pointer to the queue.
 * @Return: The size of the queue (number of elements in the queue).
 * @Complexity: O(n).
**/
size_t PQSize(const pq_t *pqueue);

/**
 * @Description: Removes all the elements from the priority queue and leaves it
 * 				empty.
 * @Parameters: pqueue - a pointer to the queue.
 * @Return: void.
 * @Complexity: O(n).
**/
void PQClear(pq_t *pqueue);

/**
 * @Description: Deletes an element (if exists) bassed on data given.
 * @Parameters: pqueue - a pointer to the queue.
 *				to_del - a void pointer to data to erase from the queue.
 *				is_match - a pointer to a function to compare the data of 
 *				to_del. The function should recieve two void pointers, and it 
 *				returns one if their data is equal or zero if not.
 * @Return: A void pointer to the data erased.
 * @Complexity: O(n).
**/
void *PQErase(pq_t *pqueue, const void *to_del, is_match_pq_t is_match);

#endif /* __PRIORITY_QUEUE_H_OL108__ */


#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h> /* size_t */

/*************** typedef *************************/

typedef struct queue queue_t;

/*************************************************/

/**
 * @Description: Creates an empty queue (containing only a dummy).
 * @Parameters: void.
 * @Return: A pointer to the new queue.
 * @Complexity: O(1).
**/
queue_t *QueueCreate(void);

/**
 * @Description: Destroys the queue (but not the data).
 * @Parameters: list - a pointer to the queue.
 * @Return: void.
 * @Complexity: O(n).
**/
void QueueDestroy(queue_t *queue);

/**
 * @Description: Inserts new data to the back of the queue.
 * @Parameters: queue - a pointer to the queue.
 *              data - a void pointer to to the new data.
 * @Return: zero if the insertion was successful, non-zero if not.
 * @Notes: The data is copied by reference - only the address is stored.
 * @Complexity: O(1).
**/
int QueueEnqueue(queue_t *queue, void *data);

/**
 * @Description: Removes a given iterator from the front of the queue.
 * @Parameters: queue - a pointer to the queue.
 * @Return: A void pointer to the dequeued iterator.
 * @Notes: If iter is the dummy, returns NULL.
 * @Complexity: O(1).
**/
void *QueueDequeue(queue_t *queue);

/**
 * @Description: Get the data from an iterator.
 * @Parameters: queue - a pointer to the queue.
 * @Return: A void pointer to the data.
 * @Complexity: O(1).
**/
void *QueuePeek(queue_t *queue);

/**
 * @Description: Get the size of the queue.
 * @Parameters: queue - a pointer to the queue.
 * @Return: The size of the queue (=number of elements in the queue).
 * @Complexity: O(n).
**/
size_t QueueGetSize(queue_t *queue);

/**
 * @Description: Check if the queue is empty (only consists of dummy tail).
 * @Parameters: queue - a pointer to the queue.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
int QueueIsEmpty(queue_t *queue);

/**
 * @Description: Concatenates two queues.
 * @Parameters: queue_t - two pointers to the queue.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
queue_t *QueueAppend(queue_t *dest, queue_t *src); 

#endif /* __QUEUE_H__ */

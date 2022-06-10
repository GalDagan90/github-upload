/**************************************************************
* Author: Gal Dagan
* Date: 4.8.21
* Reviewer: Meital Kozhidov
*
* Description: Implementation of a queue 
*
* InfintyLabs OL108
**************************************************************/

#include <assert.h>	/* assert()			*/
#include <stdlib.h> /* malloc(), free() */

#include "queue.h" 
#include "singly_linked_list.h"

struct queue
{
    slist_t *head;
};

queue_t *QueueCreate(void)
{
	queue_t *new_queue = (queue_t *)malloc(sizeof(queue_t));
	
	if (NULL == new_queue)
	{
		return NULL;
	}

	new_queue->head = SListCreate();
	
	return new_queue;
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);
	
	SListDestroy(queue->head);
	free(queue);	
}

int QueueEnqueue(queue_t *queue, void *data)
{
	int status = 0;
	slist_iter_t *iter = NULL;
	
	assert(NULL != queue);
	
	iter = GetDummyTailFromList(queue->head);
	status = SListInsertBefore(iter, data);

	return (status);
}

void *QueueDequeue(queue_t *queue)
{
	slist_iter_t *iter = NULL;
	void *data = NULL;
	
	assert(NULL != queue);
	
	iter = SListGetHead(queue->head);
	data = SListGetData(iter);
	SListRemove(iter);
	
	return data;
}

void *QueuePeek(queue_t *queue)
{
	void *data = NULL;
	slist_iter_t *iter = NULL;
	
	assert(NULL != queue);
	
	iter = SListGetHead(queue->head);
	data = SListGetData(iter);
	
	return data;
}

size_t QueueGetSize(queue_t *queue)
{
	assert(NULL != queue);
	
	return SListGetSize(queue->head);
}

int QueueIsEmpty(queue_t *queue)
{
	assert(NULL != queue);
	
	return SListIsEmpty(queue->head);
}




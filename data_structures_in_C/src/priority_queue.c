/**************************************************************
* Author: Gal Dagan
* Date: 12.08.21
* Reviewer: Eyal Dafni
*
* Description: Implementation of Priority queue
*
* InfintyLabs OL108
**************************************************************/
#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() 		*/

#include "priority_queue.h"
#include "heap.h"

struct pq
{
	heap_t *queue; 
};

pq_t *PQCreate(pq_cmp_priority_t cmp_priority)
{
	pq_t *new_pqueue = (pq_t *)malloc(sizeof(pq_t));
	
	if (NULL == new_pqueue)
	{
		return NULL;
	}
	
	new_pqueue->queue = HeapCreate(cmp_priority);
	if (NULL == new_pqueue->queue)
	{
		free(new_pqueue);
		new_pqueue = NULL;
		return NULL;
	}
	
	return (new_pqueue);
}

void PQDestroy(pq_t *pqueue)
{
	assert(NULL != pqueue);
	
	HeapDestroy(pqueue->queue);
	pqueue->queue = NULL;
	free(pqueue);
	pqueue = NULL;
}

int PQEnqueue(pq_t *pqueue, const void *data)
{	
	assert(NULL != pqueue);

	return (HeapPush(pqueue->queue, data));
}

void *PQDequeue(pq_t *pqueue)
{
	assert(NULL != pqueue);
	
	return HeapPop(pqueue->queue);
}

void *PQPeek(const pq_t *pqueue)
{
	assert(NULL != pqueue);
	
	return HeapPeek(pqueue->queue);
}

int PQIsEmpty(const pq_t *pqueue)
{
	assert(NULL != pqueue);
	
	return HeapIsEmpty(pqueue->queue);
}

size_t PQSize(const pq_t *pqueue)
{
	assert(NULL != pqueue);
	
	return HeapSize(pqueue->queue);
}

void PQClear(pq_t *pqueue)
{
	assert(NULL != pqueue);
	
	while (!PQIsEmpty(pqueue))
	{
		PQDequeue(pqueue);
	}
}

void *PQErase(pq_t *pqueue, const void *to_del, is_match_pq_t is_match)
{	
	assert(NULL != pqueue);
	
	return HeapRemove(pqueue->queue, to_del, is_match);
}
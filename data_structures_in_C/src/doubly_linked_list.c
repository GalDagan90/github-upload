/**************************************************************
* Author: Gal Dagan
* Date: 09.08.21
* Reviewer: Eyal Dafni
*
* Description: Implementation of Doublly linked list data structure
*
* InfintyLabs OL108
**************************************************************/

#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() 		*/

#include "doubly_linked_list.h"

/******************* Struct defenitions ************************/

struct dlist_node
{
    void *data;
    struct dlist_node *next;
    struct dlist_node *prev;
};

struct dlist
{
	struct dlist_node head;
	struct dlist_node tail;
};

enum status
{
	SUCCESS = 0
};

/******************* Static function declaration ***************************/

static dlist_iter_t GetDummyIter(dlist_iter_t iter);

/***************************************************************************/

dlist_t *DListCreate(void)
{
	dlist_t *new_dlist = (dlist_t *)malloc(sizeof(dlist_t));
	
	if (NULL == new_dlist)
	{
		return NULL;
	}
	
	new_dlist->head.data = NULL;
	new_dlist->tail.data = NULL;
	(new_dlist->head).next = &(new_dlist->tail);
	new_dlist->head.prev = NULL;
	new_dlist->tail.next = NULL;
	new_dlist->tail.prev = &(new_dlist->head);
	
	return new_dlist;
}

void DListDestroy(dlist_t *list)
{
	dlist_node_t *curr = NULL;
	dlist_node_t *next = NULL;
	
	assert(NULL != list);
	
	curr = (list->head).next;
	next = curr->next;
	 
	for (; NULL != next; curr = next, next = next->next)
	{
		free(curr);
	}
	
	curr = NULL;
	free(list);
	list = NULL;
}

size_t DListGetSize(const dlist_t *list)
{
	size_t counter = 0;
	dlist_node_t *runner = NULL;
	
	assert(NULL != list);
	
	runner = (list->head).next;
	
	for (;NULL != runner->next; runner = runner->next, ++counter)
	{
		/* empty loop */
	}
	
	return counter;

}

int DListIsEmpty(const dlist_t *list)
{
	assert(NULL != list);
	
	return (0 == DListGetSize(list));
}

dlist_iter_t DListGetHead(dlist_t *list)
{
	dlist_iter_t iter = {NULL};
	
	assert(NULL != list);
	
	iter.internal = (list->head).next;
	
	return iter;
}

dlist_iter_t DListGetEnd(dlist_t *list)
{
	dlist_iter_t iter = {NULL};
	
	assert(NULL != list);
	
	iter.internal = &(list->tail);
	
	return iter;
}

dlist_iter_t DListGetNext(dlist_iter_t iter)
{
	assert(NULL != iter.internal);
	
	if (NULL != (iter.internal)->next)
	{
		iter.internal = (iter.internal)->next;
	}
	
	return iter;
}

dlist_iter_t DListGetPrev(dlist_iter_t iter)
{
	assert(NULL != iter.internal);
	
	if (NULL != ((iter.internal)->prev)->prev)
	{
		iter.internal = (iter.internal)->prev;
	}
	
	return iter;
}

void *DListGetData(dlist_iter_t iter)
{
	assert(NULL != iter.internal);
	
	return (iter.internal)->data;
}

dlist_iter_t DListInsertBefore(dlist_iter_t iter, const void *data)
{
	dlist_iter_t temp_iter = {NULL};
	dlist_node_t *new_node = NULL;
	
	assert(NULL != iter.internal);
	assert(NULL != iter.internal->prev);
	
	new_node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	
	if (NULL == new_node)
	{
		return GetDummyIter(iter);
	}
	
	new_node->data = (void *)data;
	new_node->prev = (iter.internal)->prev;
	new_node->next = iter.internal;
	(iter.internal)->prev = new_node;
	(new_node->prev)->next = new_node;
	
	temp_iter.internal = new_node;
	
	return temp_iter;
}

dlist_iter_t DListRemove(dlist_iter_t iter)
{	
	dlist_iter_t next_iter = {NULL};
	
	assert(NULL != iter.internal);
	
	if (NULL == (iter.internal)->next || NULL == (iter.internal)->prev)
	{
		return iter;
	}
	
	next_iter = DListGetNext(iter);
	(iter.internal)->next->prev = (iter.internal)->prev;
	(iter.internal)->prev->next = (iter.internal)->next;
	free(iter.internal);
	
	return next_iter;
}

dlist_iter_t DListPushBack(dlist_t *list, const void *data)
{	
	assert(NULL != list);
	
	return DListInsertBefore(DListGetEnd(list), data);
}

dlist_iter_t DListPushFront(dlist_t *list, const void *data)
{	
	assert(NULL != list);
	
	return DListInsertBefore(DListGetHead(list), data);
}

void *DListPopBack(dlist_t *list)
{
	dlist_iter_t iter = {NULL};
	void *data = NULL;
	
	assert(NULL != list);
	
	iter = DListGetPrev(DListGetEnd(list));
	data = DListGetData(iter);
	DListRemove(iter);
	
	return data;
}

void *DListPopFront(dlist_t *list)
{
	dlist_iter_t iter = {NULL};
	void *data = NULL;
	
	assert(NULL != list);
	
	iter = DListGetHead(list);
	data = DListGetData(iter);
	DListRemove(iter);
	
	return data;
}

int DListIsSameIter(dlist_iter_t iter1, dlist_iter_t iter2)
{
	assert(NULL != iter1.internal);
	assert(NULL != iter2.internal);
	
	return (iter1.internal == iter2.internal);
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, 
				operation_func_t operation_func, void *param)
{
	int status = SUCCESS;
	
	assert(NULL != from.internal);
	assert(NULL != to.internal);
	
	for (; (NULL != from.internal) && !(DListIsSameIter(from, to)) && 
			(SUCCESS == status); 
			from.internal = (from.internal)->next)
	{
		status = operation_func((from.internal)->data, param);
	}
	
	return status;
}

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, 
						const void *to_find ,is_match_t is_match)
{
	assert(NULL != from.internal);
	assert(NULL != to.internal);
	
	for (; !(DListIsSameIter(from, to)) && (NULL != (from.internal)->next) && 
			!is_match((from.internal)->data, to_find); 
			from.internal = (from.internal)->next)
	{
		/* empty loop */
	}
	
	if (DListIsSameIter(from, to))
	{
		return GetDummyIter(from);
	}
	
	return from;
}

int DListMultiFind(dlist_iter_t from, dlist_iter_t to, const void *to_find, 
					is_match_t is_match, 
					dlist_t *output_list)
{
	int status = SUCCESS;
	dlist_iter_t insert_iter = {NULL};
	
	assert(NULL != from.internal);
	assert(NULL != to.internal);
	
	for (; !DListIsSameIter(from, to) && (NULL != (from.internal)->next) 
			&& (SUCCESS == status); 
			from.internal = (from.internal)->next)
	{
		if (is_match((from.internal)->data, to_find))
		{
			insert_iter = DListPushBack(output_list, (from.internal)->data);
			status = (DListIsSameIter(insert_iter, DListGetEnd(output_list)));
		}
	}
	
	return status;
}

dlist_iter_t DListSplice(dlist_iter_t from, dlist_iter_t to, dlist_iter_t dest)
{
	dlist_iter_t temp = {NULL};
	
	temp.internal = (to.internal)->prev;
	
	if (!DListIsSameIter(from,to))
	{
		(to.internal)->prev = (from.internal)->prev;
		(from.internal)->prev->next = to.internal;
		(from.internal)->prev = (dest.internal)->prev;
		(dest.internal)->prev->next = from.internal;
		(dest.internal)->prev = temp.internal;
		(temp.internal)->next = dest.internal;
	}
	
	return dest;
}


/****************** Static helper functions *******************************/

static dlist_iter_t GetDummyIter(dlist_iter_t iter)
{	
	for(; NULL != (iter.internal)->next; iter.internal = (iter.internal)->next)
	{
		/* empty loop */
	}
	
	return iter;
}


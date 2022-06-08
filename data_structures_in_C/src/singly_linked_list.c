/**************************************************************
* Author: Gal Dagan
* Date: 2.8.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Singly linked list data structure
*
* InfintyLabs OL108
**************************************************************/

#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() 		*/

#include "singly_linked_list.h"

/******************* Struct defenitions ************************/

struct node
{
    void *data;
    struct node *next;
};

struct slist
{
    struct node *head;
    struct node *dummy_tail; 
};

enum status
{
	SUCCESS = 0,
	FAILURE
};

/***************** static function declaration *****************/

static slist_iter_t *GetDummyIter(const slist_iter_t *iter);

/************* Linked List Implementation *********************/

slist_t *SListCreate(void)
{
	slist_t *new_slist = (slist_t *)malloc(sizeof(slist_t));
	
	if (NULL == new_slist)
	{
		return NULL;
	}
	
	new_slist->head = (slist_iter_t *)malloc(sizeof(slist_iter_t));
	
	if (NULL == new_slist->head)
	{
		free(new_slist);
		return NULL;
	}
	
	new_slist->head->data = (void *)new_slist;
	new_slist->head->next = NULL;
	new_slist->dummy_tail = new_slist->head;
	
	return new_slist;
}

int SListIsEmpty(const slist_t *list)
{
	assert(NULL != list);
	
	return (0 == SListGetSize(list));
}

size_t SListGetSize(const slist_t *list)
{
	size_t counter = 0;
	slist_iter_t *runner = NULL;
	
	assert(NULL != list);
	
	runner = list->head;
	
	for(;NULL != runner->next; runner = runner->next, ++counter)
	{
		/* empty loop */
	}
	
	return counter;
}

int SListInsertBefore(slist_iter_t *iter, void *data)
{
	int status = SUCCESS;
	slist_iter_t *new_iter = NULL;
	
	assert(NULL != iter);
	
	new_iter = (slist_iter_t *)malloc(sizeof(slist_iter_t));
	
	if (NULL == new_iter)
	{
		status = FAILURE;
		return status;
	}
	
	new_iter->data = iter->data;
	new_iter->next = iter->next;
	iter->data = data;
	iter->next = new_iter;
	
	if (NULL == new_iter->next)
	{
		((slist_t *)new_iter->data)->dummy_tail = new_iter;
	}
	
	return status;
}

void *SListGetData(const slist_iter_t *iter)
{
	assert(NULL != iter);
	
	return (NULL != iter->next)? iter->data: NULL;
}

slist_iter_t *SListGetHead(const slist_t *list)
{
	assert(NULL != list);
	
	return list->head;
}


slist_iter_t *SListGetNext(const slist_iter_t *iter)
{
	assert(NULL != iter);
	
	return iter->next;
}

slist_iter_t *SListGetEnd(const slist_t *list)
{
	slist_iter_t *list_runner = NULL;
	
	assert(NULL != list);
	
	list_runner = list->head;
	
	if (0 == SListIsEmpty(list))
	{
		for(; NULL != list_runner->next->next; list_runner = list_runner->next)
		{
			/* empty loop */
		}
	}
	
	return list_runner;
}

slist_iter_t *SListRemove(slist_iter_t *iter)
{
	assert(NULL != iter);
	
	if (NULL != iter->next)
	{
		slist_iter_t *temp = iter->next;
		iter->data = temp->data;
		iter->next = temp->next;
		free(temp);
	}
	
	if (NULL == iter->next)
	{
		((slist_t *)(iter->data))->dummy_tail = iter;
	}
	
	return iter;
}

int SListIsIterEqual(const slist_iter_t *iter1, const slist_iter_t *iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}


slist_iter_t *SListFind(const slist_iter_t *iter_from, 
						const slist_iter_t *iter_to, 
						const slist_iter_t *iter, 
						cmp_func_ptr cmp_func)
{
	slist_iter_t *iter_runner = NULL;
	
	assert(NULL != iter_from);
	assert(NULL != iter_to);
	assert(NULL != iter);
	
	iter_runner = (slist_iter_t *)iter_from;
	
	for (; iter_runner != iter_to && !cmp_func(iter->data, iter_runner->data); 
		iter_runner = iter_runner->next)
	{
		/* empty loop */
	}
	
	if (iter_runner != iter_to && cmp_func(iter->data, iter_runner->data))
	{
		iter_runner = (slist_iter_t *)iter;
	}
	else
	{
		iter_runner = GetDummyIter(iter_runner);
	}
	
	return iter_runner; 
}

int SListForEach(slist_iter_t *iter_from, 
				slist_iter_t *const iter_to, 
				operation_func_ptr operation_func)
{
	int status = SUCCESS;
	
	assert(NULL != iter_from);
	assert(NULL != iter_to);
	
	for (; iter_from != iter_to->next && NULL != iter_from && SUCCESS ==status; 
		iter_from = iter_from->next)
	{
		status = operation_func(iter_from->data);
	}
	
	return status;
}

void SListDestroy(slist_t *list)
{
	slist_iter_t *temp = NULL;
	slist_iter_t *next_head = NULL;
	
	assert(NULL != list);
	
	temp = list->head;
	next_head = temp->next;
	 
	for(; NULL != next_head; temp = next_head, next_head = next_head->next)
	{
		free(temp);
	}
	free(temp);
	free(list);
}

static slist_iter_t *GetDummyIter(const slist_iter_t *iter)
{
	assert(NULL != iter);
	
	for(; NULL != iter->next; iter = iter->next)
	{
		/* empty loop */
	}
	
	return (slist_iter_t *)iter;
}

slist_iter_t *GetDummyTailFromList(slist_t *list)
{
	assert(NULL != list);
	
	return list->dummy_tail;	
}



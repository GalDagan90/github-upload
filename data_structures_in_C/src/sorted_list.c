/**************************************************************
* Author: Gal Dagan
* Date: 09.08.21
* Reviewer: Meital Kozhidov
*
* Description: Implementation of Sorted list data structure
*
* InfintyLabs OL108
**************************************************************/

#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() 		*/

#include "sorted_list.h"
#include "doubly_linked_list.h"
/******************* Struct defenitions ************************/

struct sorted_list
{
	dlist_t *list;
	sort_cmp_func_t cmp_func;
};

/**************** Static function declaration ****************************/

static sort_list_iter_t FindIterLocation(sort_list_t *list, const void *data);
static sort_list_iter_t GetPlace(sort_list_t *dest, sort_list_iter_t move_to 
										,void *data);
/*************************************************************************/

sort_list_t *SortListCreate(sort_cmp_func_t cmp_func)
{
	sort_list_t *new_list = (sort_list_t *)malloc(sizeof(sort_list_t));
	
	if (NULL == new_list)
	{
		return NULL;
	}
	
	new_list->list = DListCreate();
	
	if (NULL == new_list->list)
	{
		free(new_list);
		return NULL;
	}
	
	new_list->cmp_func = cmp_func;
	
	return new_list;	
}

void SortListDestroy(sort_list_t *list)
{
	assert(NULL != list);
	
	DListDestroy(list->list);
	free(list);
	list = NULL;
}

size_t SortListSize(const sort_list_t *list)
{
	assert(NULL != list);
	
	return DListGetSize(list->list);
}

int SortListIsEmpty(const sort_list_t *list)
{
	assert(NULL != list);
	
	return DListIsEmpty(list->list);
}

sort_list_iter_t SortListGetBegin(const sort_list_t *list)
{
	sort_list_iter_t iter = {NULL}; 

	assert(NULL != list);

	iter.internal = DListGetHead(list->list);

	return iter;
}

sort_list_iter_t SortListGetEnd(const sort_list_t *list)
{
	sort_list_iter_t iter = {NULL}; 

	assert(NULL != list);

	iter.internal = DListGetEnd(list->list);

	return iter;
}

sort_list_iter_t SortListGetNext(sort_list_iter_t iter)
{
	assert(NULL != iter.internal.internal);
	
	iter.internal = DListGetNext(iter.internal);

	return iter; 
}

sort_list_iter_t SortListGetPrev(sort_list_iter_t iter)
{
	assert(NULL != iter.internal.internal);
	
	iter.internal = DListGetPrev(iter.internal);

	return iter; 
}

void *SortListGetData(sort_list_iter_t iter)
{
	void *data = NULL;
	
	assert(NULL != iter.internal.internal);
	
	data = DListGetData(iter.internal);

	return data; 
}

sort_list_iter_t SortListInsert(sort_list_t *list, const void *data)
{
	sort_list_iter_t iter = {NULL};
	
	assert(NULL != list);
	
	iter = FindIterLocation(list, data);
	
	iter.internal = DListInsertBefore(iter.internal, data);
	
	return iter;
}

sort_list_iter_t SortListRemove(sort_list_iter_t iter)
{
	assert(NULL != iter.internal.internal);
	
	iter.internal = DListRemove(iter.internal);
	
	return iter;
}

int SortListIsSameIter(sort_list_iter_t iter1, sort_list_iter_t iter2)
{
	assert(NULL != iter1.internal.internal);
	assert(NULL != iter2.internal.internal);
	
	return (DListIsSameIter(iter1.internal, iter2.internal));
}

void *SortListPopBegin(sort_list_t *list)
{
	assert(NULL != list);
	
	return DListPopFront(list->list);
}

void *SortListPopEnd(sort_list_t *list)
{
	assert(NULL != list);
	
	return DListPopBack(list->list);
}

int SortListForEach(sort_list_iter_t from, sort_list_iter_t to, 
					operation_func_t operation_func, void *param)
{	
	return DListForEach(from.internal, to.internal, operation_func, param);
}

sort_list_iter_t SortListFind(const sort_list_t *list, const void *to_find)
{
	sort_list_iter_t result_iter = {NULL};
	
	assert(NULL != list);
	
	result_iter = FindIterLocation((sort_list_t *)list, to_find);
	
	if (!SortListIsSameIter(result_iter, SortListGetEnd(list)) && 
		0 == list->cmp_func(SortListGetData(result_iter), to_find))
	{
		return result_iter;
	}
	
	return SortListGetEnd(list);
}

sort_list_t *SortListMerge(sort_list_t *dest, sort_list_t *src)
{
	sort_list_iter_t dest_runner = {NULL};
	sort_list_iter_t src_runner = {NULL};
	
	assert(NULL != dest);
	assert(NULL != src);
	
	dest_runner = SortListGetBegin(dest);

	while (!SortListIsEmpty(src))
	{
		src_runner = SortListGetBegin(src);
		
		dest_runner = GetPlace(dest, dest_runner, SortListGetData(src_runner));
		
		DListSplice(src_runner.internal, SortListGetNext(src_runner).internal,
					dest_runner.internal);
	}
	
	return dest;
}

sort_list_iter_t SortListFindIf(sort_list_iter_t from, sort_list_iter_t to,
								const void *to_find, is_match_t is_match)
{
    sort_list_iter_t results = {NULL};

    assert(NULL != from.internal.internal);
    assert(NULL != to.internal.internal);

    results.internal = DListFind(from.internal, to.internal, to_find, is_match);

    return results;
}

static sort_list_iter_t FindIterLocation(sort_list_t *list, const void *data)
{
	sort_list_iter_t iter = {NULL};
	sort_list_iter_t tail_iter = {NULL};
	
	assert(NULL != list);
	
	iter = SortListGetBegin(list);
	tail_iter = SortListGetEnd(list);
	
	for (; !SortListIsSameIter(iter, tail_iter) && 
		list->cmp_func(data, SortListGetData(iter)) > 0;
		iter = SortListGetNext(iter))
	{
		/* empty loop */
	}
	
	return iter;
}

static sort_list_iter_t GetPlace(sort_list_t *dest, sort_list_iter_t move_to 
								,void *data)
{
	for (; !SortListIsSameIter(move_to, SortListGetEnd(dest)) && (
		0 >= dest->cmp_func(SortListGetData(move_to), data)); 
		move_to = SortListGetNext(move_to))
	{
		/* empty loop */
	}
		
	return move_to;
}


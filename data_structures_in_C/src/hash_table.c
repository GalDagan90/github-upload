/****************************************************************************
*Author : Gal Dagan
*Date : 14.09.21
*Reviewer : Liel Sananes
*
*Description : Implementation of hash table ADT. 
*
*Lab : OL108
****************************************************************************/

#include <stdlib.h>	/* malloc(), free() */ 
#include <assert.h> /* assert() */

#include "hash_table.h"
#include "doubly_linked_list.h" /* DLL inner functions */

typedef enum status
{
	HASH_SUCCESS = 0,
	DUP_KEY_ERROR,
	ALLOC_ERROR,
	FOR_EACH_ERROR
}hash_status_t;

struct hash_table
{
	hash_func_t hash_func;
	hash_is_match_t is_match;
	dlist_t **dlist;
	size_t size;
};

/******************************************************************************
*							Static function decleration						  *
******************************************************************************/

static size_t GetHashValue(const hash_table_t *table, const void *data);
static dlist_iter_t FindIterInDLL(dlist_t *dlist, const hash_table_t *table,
									const void *data);
static void ReHash(hash_table_t *table);
static int SizeCount(void *data, void *param);
								
/*****************************************************************************/

hash_table_t *HashTableCreate(size_t hash_size, hash_is_match_t is_match, 
								hash_func_t hash_func)
{
	hash_table_t *new_hash_t = NULL;
	size_t i = 0;
	
	assert(0 != hash_size);
	assert(NULL != is_match);
	assert(NULL != hash_func);
	
	new_hash_t = (hash_table_t *)malloc(sizeof(hash_table_t));
	if (NULL == new_hash_t)
	{
		return NULL;
	}
	
	new_hash_t->dlist = (dlist_t **)malloc(hash_size * sizeof(dlist_t *));
	if (NULL == new_hash_t->dlist)
	{
		free(new_hash_t);
		new_hash_t = NULL;
		
		return NULL;
	}
	
	for (; i < hash_size; ++i)
	{
		new_hash_t->dlist[i] = DListCreate();
		if (NULL == new_hash_t->dlist[i])
		{
			HashTableDestroy(new_hash_t);
			
			return NULL;
		}
	}

	new_hash_t->hash_func = hash_func;
	new_hash_t->is_match = is_match;
	new_hash_t->size = hash_size;
	
	return new_hash_t;
}

void HashTableDestroy(hash_table_t *table)
{
	size_t i = 0;
	
	assert(NULL != table);
	
	for (; i < table->size; ++i)
	{
		if (NULL != table->dlist[i])
		{
			DListDestroy(table->dlist[i]);
			table->dlist[i] = NULL;
		}
	}
	
	free(table->dlist);
	table->dlist = NULL;
	
	free(table);
	table = NULL;
}

int HashTableInsert(hash_table_t *table, const void *data)
{
	size_t key = 0;
	int status = DUP_KEY_ERROR;
	dlist_iter_t insert_iter = {NULL};

	assert(NULL != table);
	
	if (NULL != HashTableFind(table, data))
	{
		return status;
	}
	
	key = GetHashValue(table, data);
	insert_iter = DListPushBack(table->dlist[key], (void *)data);
	if (!DListIsSameIter(DListGetEnd(table->dlist[key]), insert_iter))
	{
		status = HASH_SUCCESS;
	}
	
	return status;	
}

void *HashTableRemove(hash_table_t *table, const void *data)
{
	size_t key = 0;
	dlist_iter_t iter_to_del = {NULL};
	void *removed_data = NULL;
	
	assert(NULL != table);
	
	if (NULL != HashTableFind(table, data))
	{
		key = GetHashValue(table, data);
		iter_to_del = FindIterInDLL(table->dlist[key], table, data);
		removed_data = DListGetData(iter_to_del);
		DListRemove(iter_to_del);
	}
	
	return removed_data;
}

size_t HashTableGetSize(const hash_table_t *table)
{
	size_t tot_elements = 0;
	
	assert(NULL != table);
	
	HashTableForEach((hash_table_t *)table, SizeCount, &tot_elements);
	
	return tot_elements;
}

int HashTableIsEmpty(const hash_table_t *table)
{
	assert(NULL != table);
	
	return (0 == HashTableGetSize(table));
}

void *HashTableFind(const hash_table_t *table, const void *data)
{
	size_t key = 0;
	dlist_iter_t found_iter = {NULL};
	void *found_data = NULL;
	
	assert(NULL != table);
	assert(NULL != data);
	
	key = GetHashValue(table, data);
	
	found_iter = FindIterInDLL(table->dlist[key], table, data);
	found_data = DListGetData(found_iter);
	
	if (!DListIsSameIter(found_iter, DListGetEnd(table->dlist[key])))
	{
		DListRemove(found_iter);
		DListPushFront(table->dlist[key], found_data);
	}
	
	return found_data;
}

int HashTableForEach(hash_table_t *table, 
							   hash_table_oper_func_t oper_func, void *param)
{
	int status = HASH_SUCCESS;
	size_t key = 0;
	
	assert(NULL != table);
	assert(NULL != oper_func);
	
	for (; (HASH_SUCCESS == status) && (key < table->size); ++key)
	{	
		status = DListForEach(DListGetHead(table->dlist[key]), 
							DListGetEnd(table->dlist[key]), oper_func, param);
	}
	
	ReHash(table);
	
	return status;
}			

/******************************************************************************
*							Static functions								  *
*******************************************************************************/

static size_t GetHashValue(const hash_table_t *table, const void *data)
{
	return ((table->hash_func((void *)data)) % table->size);
}

static dlist_iter_t FindIterInDLL(dlist_t *dlist, const hash_table_t *table,
									const void *data)
{
	dlist_iter_t start_iter = DListGetHead(dlist);
	dlist_iter_t end_iter = DListGetEnd(dlist);
	
	return (DListFind(start_iter, end_iter, (void *)data, table->is_match));
}

static int SizeCount(void *data, void *param)
{
	hash_status_t status = HASH_SUCCESS;
	
	if (NULL != data)
	{
		++(*(size_t *)param);
	}
	
	return status;
}

static void ReHash(hash_table_t *table)
{
	size_t key = 0;
	
	for (; key < table->size; ++key)
	{
		dlist_iter_t runner = DListGetHead(table->dlist[key]);
		dlist_iter_t end_iter = DListGetEnd(table->dlist[key]);
		for (; !DListIsSameIter(runner, end_iter); )
		{
			void *data = DListGetData(runner);
			if (key != GetHashValue(table, data))
			{
				dlist_iter_t temp = DListGetNext(runner);
				DListRemove(runner);
				DListPushFront(table->dlist[GetHashValue(table, data)], data);
				runner = temp;
			}
			else
			{
				runner = DListGetNext(runner);
			}
		}
	}
}


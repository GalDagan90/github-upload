/**************************************************************
* Author: Gal Dagan
* Date: 19.08.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Fixed size allocator
*
* InfintyLabs OL108
**************************************************************/
#include <assert.h> /* assert() */

#include "fsa.h"

#define ALIGNMENT sizeof(size_t)
#define ROUND_TO_ALIGNED_NUM(x) ( (x % ALIGNMENT) ? \
								(x - x % ALIGNMENT + ALIGNMENT) : x )

struct fsa
{
	char *curr_free;
	size_t block_size;
	size_t num_of_blocks;
};


fsa_t *FSACreate(void *pool_ptr, size_t block_size, size_t num_of_blocks)
{
    fsa_t new_fsa = {NULL, 0 , 0};

    assert(NULL != pool_ptr);
    
	new_fsa.block_size = ROUND_TO_ALIGNED_NUM(block_size);
	new_fsa.num_of_blocks = num_of_blocks;
	*(fsa_t *)pool_ptr = new_fsa;
	
	FSAClear((fsa_t *)pool_ptr);

    return pool_ptr;
}

void FSAClear(fsa_t *pool)
{
	char *runner = NULL;
	size_t counter = 0;
	
	assert(NULL != pool);
	
	pool->curr_free = (char *)(pool +  1);
	runner = pool->curr_free;
	
	for (; counter < pool->num_of_blocks - 1; ++counter,
		runner += pool->block_size)
	{
		*((char **)runner) = (runner + pool->block_size);
	}
	
	*((char **)runner) = NULL;
}

void *FSAAlloc(fsa_t *pool)
{
	void *alloc_block = NULL;
	
	assert(NULL != pool);
	
	if (0 != FSACountFree(pool))
	{
		alloc_block = pool->curr_free;
		pool->curr_free = *((char **)(pool->curr_free));
	}
	
	return alloc_block;
}

void FSAFree(fsa_t *pool, void *block_to_free)
{
	assert(NULL != pool);
	assert(NULL != block_to_free);
	
	if (FSACountFree(pool) < pool->num_of_blocks 
		&& pool->curr_free != block_to_free)
	{
		*((char **)block_to_free) = pool->curr_free;
		pool->curr_free = block_to_free;
	}
}

size_t FSACountFree(fsa_t *pool)
{
	char *runner = NULL;
	size_t counter = 0;
	
	assert(NULL != pool);
	
	runner = pool->curr_free;
	
	for (; NULL != runner && counter < pool->num_of_blocks; ++counter)
	{
		runner = *((char **)(runner));
	}
	
	return counter;
}

size_t FSASuggestSize(size_t block_size,size_t num_of_blocks)
{
	block_size = ROUND_TO_ALIGNED_NUM(block_size);
	 
	return (block_size * num_of_blocks + sizeof(fsa_t));
}

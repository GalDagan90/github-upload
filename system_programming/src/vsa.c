/******************************************************************************
 * Author: Gal Dagan                                                      	  *
 * Reviewer: Liel Sananes                                              		  *
 * Created: 21.08.2021                                                        *
 *                                                                            *
 * Description: implementing Fix Size Alocation                               *
 *                                                                            *
 *                                                                            *
 * Infinity Labs OL108                                                        *
 ******************************************************************************/
#include <assert.h> /* assert() */

#include "vsa.h"

#define ALIGNMENT sizeof(size_t)
#define ROUND_TO_ALIGNED_NUM(x) ( (x % ALIGNMENT) ? \
								(x - x % ALIGNMENT + ALIGNMENT) : x )
#define ABS(x) ( ((x) < 0) ? (-x) : (x) )
#define MAX(a, b) ( ((a) > (b))? (a) : (b) )
#define VSA_SIZE sizeof(vsa_t)
#define START_HDR(vsa) ((blk_hdr_t *)(vsa + 1))
#define NEXT_HDR(vsa, n_bytes) ((blk_hdr_t *)((char *)vsa + n_bytes))
#define HDR_SIZE sizeof(blk_hdr_t)
#define MAGIC_NUM 73

struct vsa
{
	char *pool_end;
};

struct block_header
{
	long block_size;
	#ifndef NDEBUG
		int magic_number;
	#endif 	/* NDEBUG */
};

/**************************** Static Functions *******************************/

static blk_hdr_t *FindFreeAllocPoint(vsa_t *vsa, size_t *n_bytes);
static size_t IsFreeBlock(vsa_t *vsa, blk_hdr_t *start_point, size_t tot_bytes);
static void AllocBlk(vsa_t *vsa, blk_hdr_t *alloc_blk, size_t tot_bytes);
static void MergeFreeBlocks(blk_hdr_t *alloc_blk, size_t free_bytes);
static void SplitBlk(vsa_t *vsa, blk_hdr_t *alloc_blk, size_t n_bytes);
static size_t CountFreeBytes(vsa_t *vsa, blk_hdr_t *start_point);
static blk_hdr_t *GetNext(blk_hdr_t *runner, long jump);

/*****************************************************************************/

vsa_t *VSAInit(void *pool_ptr, size_t pool_size)
{
	vsa_t new_vsa = {NULL};
	
    assert(NULL != pool_ptr);
    
	new_vsa.pool_end = (char *)pool_ptr + pool_size;
	*(vsa_t *)pool_ptr = new_vsa;
	
	VSAClear(pool_ptr);
	
	return pool_ptr;
}

void *VSAAlloc(vsa_t *vsa, size_t n_bytes)
{
	blk_hdr_t *alloc_point = NULL;
	
	assert(NULL != vsa);
	
	alloc_point = FindFreeAllocPoint(vsa, &n_bytes);
	if (NULL != alloc_point)
	{
		AllocBlk(vsa, alloc_point, n_bytes);
		
		#ifndef NDEBUG
			alloc_point->magic_number = MAGIC_NUM;
		#endif 
		
		alloc_point = (blk_hdr_t *)((char *)alloc_point + HDR_SIZE);
	}
	
	return alloc_point;
}

void VSAFree(void *block_to_free)
{
	blk_hdr_t *hdr = NULL;
	
	assert(NULL != block_to_free);
	
	hdr = (blk_hdr_t *)((char *)(block_to_free) - HDR_SIZE);
	
	#ifndef NDEBUG
		assert(MAGIC_NUM == hdr->magic_number);
	#endif 
	
	if (hdr->block_size > 0)
	{
		hdr->block_size *= -1;
	}
}

void VSAClear(vsa_t *vsa)
{	
	assert(NULL != vsa);
	
	START_HDR(vsa)->block_size = -1 * (vsa->pool_end - (char *)vsa - VSA_SIZE);
	
	#ifndef NDEBUG
		START_HDR(vsa)->magic_number = MAGIC_NUM;
	#endif
}

size_t VSALargestFreeBlock(vsa_t *vsa)
{
	blk_hdr_t *runner = NULL;
	size_t sum_bytes = 0;
	size_t bytes_jump = 0;
	
	assert(NULL != vsa);
	
	runner = START_HDR(vsa);
	
	for (; (char *)runner < vsa->pool_end; runner = GetNext(runner, bytes_jump))
	{
		if (runner->block_size > 0)
		{
			bytes_jump = runner->block_size;
		}
		else
		{
			bytes_jump = CountFreeBytes(vsa, runner);
			sum_bytes = (bytes_jump > sum_bytes)? bytes_jump : sum_bytes; 
		}
	}
	
	return sum_bytes;
}

/*****************************************************************************
*					     Static helper functions						     *
******************************************************************************/

static blk_hdr_t *GetNext(blk_hdr_t *runner, long jump)
{
	return (blk_hdr_t *)((char *)runner + ABS(jump));
}

static blk_hdr_t *FindFreeAllocPoint(vsa_t *vsa, size_t *n_bytes)
{
	blk_hdr_t *runner = NULL;
	
	assert(NULL != vsa);
	
	*n_bytes = ROUND_TO_ALIGNED_NUM(*n_bytes) + HDR_SIZE;
	runner = START_HDR(vsa);
	
	for (; ((char *)runner < vsa->pool_end) && 
			!IsFreeBlock(vsa, runner, *n_bytes); 
		runner = GetNext(runner,runner->block_size))
	{
		/* empty loop */
	}
	
	if ((char *)runner < vsa->pool_end )
	{
		return runner;
	}
	
	return NULL;
}

static size_t IsFreeBlock(vsa_t *vsa, blk_hdr_t *start_point, size_t tot_bytes)
{
	blk_hdr_t *runner = NULL;
	size_t sum_free_bytes = 0;
	
	assert(NULL != start_point);
	
	runner = start_point;
	
	for (; (char *)runner != vsa->pool_end && sum_free_bytes < tot_bytes 
			&& runner->block_size < 0; 
		runner = GetNext(runner, runner->block_size))
	{
		sum_free_bytes += ABS(runner->block_size);
	}
	
	return (sum_free_bytes >= tot_bytes);
}
 
static void AllocBlk(vsa_t *vsa, blk_hdr_t *alloc_blk, size_t n_bytes)
{
	size_t bytes_free = CountFreeBytes(vsa, alloc_blk);
	
	MergeFreeBlocks(alloc_blk, bytes_free);
	
	if (bytes_free - n_bytes > HDR_SIZE)
	{
		SplitBlk(vsa, alloc_blk, n_bytes);
	}
	
	alloc_blk->block_size *= -1;
}

static void MergeFreeBlocks(blk_hdr_t *alloc_blk, size_t free_bytes)
{
	alloc_blk->block_size = -1 * free_bytes;
}

static void SplitBlk(vsa_t *vsa, blk_hdr_t *alloc_blk, size_t n_bytes)
{
	char *next_blk_after_split = (char *)alloc_blk + n_bytes;
	long bytes_left = ABS(alloc_blk->block_size) - n_bytes;
	size_t bytes_diff_from_start = next_blk_after_split - (char *)vsa;
	
	alloc_blk->block_size = -1 * n_bytes;
	NEXT_HDR(vsa, bytes_diff_from_start)->block_size = -1 * bytes_left;
	#ifndef NDEBUG
		NEXT_HDR(vsa, bytes_diff_from_start)->magic_number = MAGIC_NUM;
	#endif 	
}

static size_t CountFreeBytes(vsa_t *vsa, blk_hdr_t *start_point)
{
	blk_hdr_t *runner = NULL;
	size_t sum_free_bytes = 0;
	size_t bytes_jump = 0;

	runner = start_point;
	
	for (; (char *)runner < vsa->pool_end && runner->block_size < 0; 
		 runner = GetNext(runner, bytes_jump))
	{
		bytes_jump = ABS(runner->block_size);
		sum_free_bytes += bytes_jump;
	}
	
	return sum_free_bytes;
}



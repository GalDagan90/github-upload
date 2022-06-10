/****************************************************************************
*Author : Gal Dagan
*Date : 19/08/21
*Reviewer : Aharon Luzon
*
*Description : implementation of FSA
*
*Lab : ol108
****************************************************************************/

#ifndef __FSA_H_OL108_ILRD__
#define __FSA_H_OL108_ILRD__

#include <stddef.h> /* size_t */

typedef struct fsa fsa_t;


/**
 * @Description: Create a new fixed size allocator in a given memory pool.
 * @Parameters: pool_ptr - a void pointer to the pool.
 *              block_size - the size of each block.
 *              num_of_blockes - number of blockes that fit in the pool.
 * @Return: A pointer to the new fixed size allocator.
 * @Notes: The area pointed to by pool_ptr should have at least as much size as 
 *         returned by FSASuggestSize.
 *         If block_size is not divisible by the word size, padding will be kept
 *         between blocks.
 * @Complexity: O(n).
**/
fsa_t *FSACreate(void *pool_ptr, size_t block_size, size_t num_of_blocks);

/**
 * @Description: Clear (free) all blocks in a fixed size allocator.
 * @Parameters: pool - a pointer to the fixed size allocator.
 * @Return: void.
 * @Complexity: O(n).
**/
void FSAClear(fsa_t *pool);

/**
 * @Description: Allocate a block from a fixed size allocator.
 * @Parameters: pool - a pointer to the fixed size allocator.
 * @Return: A void pointer to the allocated block, or NULL if the pool is full.
 * @Complexity: O(1).
**/
void *FSAAlloc(fsa_t *pool);

/**
 * @Description: Free a specific block in a fixed size allocator.
 * @Parameters: pool - a pointer to the fixed size allocator.
 *              block_to_free - a pointer to the block that should be freed.
 * @Return: void.
 * @Complexity: O(1).
**/
void FSAFree(fsa_t *pool, void *block_to_free);

/**
 * @Description: Count the number of free blocks in a fixed size allocator.
 * @Parameters: pool - a pointer to the fixed size allocator.
 * @Return: The number of free blocks.
 * @Complexity: O(n).
**/
size_t FSACountFree(fsa_t *pool);

/**
 * @Description: Calculate the size needed for a fixed size allocator.
 * @Parameters: block_size - the size of each block.
 *              num_of_blockes - number of blockes that fit in the pool.
 * @Return: The suggested size.
 * @Complexity: O(1).
**/
size_t FSASuggestSize(size_t block_size,size_t num_of_blocks);

#endif /* __FSA_H_OL108_ILRD__ */


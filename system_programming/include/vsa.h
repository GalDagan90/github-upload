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

#ifndef __VSA_H_OL108_ILRD__
#define __VSA_H_OL108_ILRD__

/************************************************************/
#include <stddef.h> /* size_t */

typedef struct vsa vsa_t;
typedef struct block_header blk_hdr_t;

/*************************************************************/

/**
 * @Description: Create a new variable size allocator in a given memory pool.
 * @Parameters: pool_ptr - a void pointer to the pool.
 *              pool_size - number of bytes in the pool.
 * @Return: A pointer to the new variable size allocator.
 * @Notes: pool_ptr is not allocated or freed by the VSA functions.
 *         Not all of pool_size will be available for allocation.
 * @Complexity: O(1).
**/
vsa_t *VSAInit(void *pool_ptr, size_t pool_size); 

/**
 * @Description: Allocate a memory block from a variable size allocator.
 * @Parameters: vsa - a pointer to the variable size allocator.
 *              n_bytes - number of bytes to allocate.
 * @Return: A void pointer to the allocated block, or NULL if a block of size
 *          n_bytes is not available.
 * @Complexity: O(n) - pool_size.
**/
void *VSAAlloc(vsa_t *vsa, size_t n_bytes);

/**
 * @Description: Free a specific memory block in a variable size allocator.
 * @Parameters: block_to_free - a pointer to the block that should be freed.
 * @Return: void.
 * @Notes: Does nothing if the block is already free.
 * @Complexity: O(1).
**/
void VSAFree(void *block_to_free);

/**
 * @Description: Free all memory blocks in a variable size allocator.
 * @Parameters: vsa - a pointer to the variable size allocator.
 * @Return: void.
 * @Complexity: O(1).
**/
void VSAClear(vsa_t *vsa);

/**
 * @Description: Find the size of the largest free block in a variable size 
 *               allocator.
 * @Parameters: vsa - a pointer to the variable size allocator.
 * @Return: Size (in bytes) of the largest free block.
 * @Complexity: O(n) - pool_size.
**/
size_t VSALargestFreeBlock(vsa_t *vsa);

#endif /* __VSA_H_OL108_ILRD__ */ 


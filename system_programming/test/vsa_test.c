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
 #include <stdlib.h>	/* malloc(), free() */
#include <stdio.h> 		/*printf()	*/
#include <assert.h>		/* assert() */

#include "vsa.h"

int main()
{
	void *pool = malloc(164);
	/*void *pool_2 = malloc(10);*/
	vsa_t *vsa = VSAInit(pool, 164);
	void *block_1 = NULL;
	void *block_2 = NULL;
	void *block_3 = NULL;
	void *block_4 = NULL;
	void *block_5 = NULL;
	
	assert(156 == VSALargestFreeBlock(vsa));
	block_1 = VSAAlloc(vsa, 10);
	block_2 = VSAAlloc(vsa, 10);
	block_3 = VSAAlloc(vsa, 20);
	*(char *)block_1 = 'a';
	assert(52 == VSALargestFreeBlock(vsa));
	VSAFree(block_1);
	VSAFree(block_2);
	assert(64 == VSALargestFreeBlock(vsa));
	block_4 = VSAAlloc(vsa, 8);
	block_5 = VSAAlloc(vsa, 8);
	assert(52 == VSALargestFreeBlock(vsa));
	VSAFree(block_3);
	assert(92 == VSALargestFreeBlock(vsa));
	VSAFree(block_5);
	block_1 = VSAAlloc(vsa, 66);
	assert(44 == VSALargestFreeBlock(vsa));
	/*block_2 = VSAAlloc(vsa, 50);
	assert(NULL == block_2);*/
	VSAFree(block_4);
	
	VSAClear(vsa);
	assert(156 == VSALargestFreeBlock(vsa));
	/*VSAFree(pool_2);*/
	free(pool);
	
	return (0);	
}	

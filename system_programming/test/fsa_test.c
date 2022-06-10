/******************************************************************************
 * Author: Gal Dagan                                                      *
 * Reviewer:                                                        *
 * Created: 18.08.2021                                                        *
 *                                                                            *
 * Description: implementing Fix Size Alocation                               *
 *                                                                            *
 *                                                                            *
 * Infinity Labs OL108                                                        *
 ******************************************************************************/
#include <stdlib.h> /* malloc(), free() */
#include <stdio.h> /*printf()*/
#include <assert.h>
#include "fsa.h" 


int main ()
{
	size_t block_size = sizeof(int); 
	size_t num_of_blocks = 5;
	size_t total_size = 0;
	void *pool_ptr = NULL; 
	fsa_t *fsa = NULL;
	void *block_1 = NULL;
	void *block_2 = NULL;
	void *block_3 = NULL;
	void *block_4 = NULL;
	void *block_5 = NULL;
	void *tmp_block = NULL;
	
	total_size = FSASuggestSize(block_size , num_of_blocks);
	pool_ptr = malloc(total_size); 
	
	fsa = FSACreate(pool_ptr, block_size ,num_of_blocks);
	assert(num_of_blocks == FSACountFree(fsa));

	block_1 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_1) % sizeof (size_t));
	assert(pool_ptr <= block_1);
	assert(num_of_blocks - 1 == FSACountFree(fsa));

	block_2 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_2) % sizeof (size_t));
	assert(pool_ptr <= block_2);
	assert(block_1 != block_2);
	assert(num_of_blocks -2 == FSACountFree(fsa));

	block_3 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_3) % sizeof (size_t));
	assert(pool_ptr <= block_3);
	assert(block_1 != block_3);
	assert(block_2 != block_3);
	assert(num_of_blocks - 3 == FSACountFree(fsa));

	FSAFree(fsa, block_3);
	assert(num_of_blocks -2 == FSACountFree(fsa));

	tmp_block = FSAAlloc(fsa);
	assert(block_3 == tmp_block);
	assert(num_of_blocks - 3 == FSACountFree(fsa));
	
	FSAClear(fsa);
	assert(5 == FSACountFree(fsa));
	
	block_1 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_1) % sizeof (size_t));
	assert(pool_ptr <= block_1);
	assert((num_of_blocks - 1) == FSACountFree(fsa));

	block_2 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_2) % sizeof (size_t));
	assert(pool_ptr <= block_2);
	assert(block_1 != block_2);
	assert(num_of_blocks -2 == FSACountFree(fsa));

	block_3 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_3) % sizeof (size_t));
	assert(pool_ptr <= block_3);
	assert(block_1 != block_3);
	assert(block_2 != block_3);
	assert(num_of_blocks - 3 == FSACountFree(fsa));
	
	block_4 = FSAAlloc(fsa);
	assert(0 == ((size_t)  block_4) % sizeof (size_t));
	assert(num_of_blocks - 4 == FSACountFree(fsa));
	
	block_5 = FSAAlloc(fsa);
	assert(pool_ptr <= block_5);
	assert(num_of_blocks - 5 == FSACountFree(fsa));
	
	tmp_block = FSAAlloc(fsa);
	assert(NULL == tmp_block);
	assert(0 == FSACountFree(fsa));
	
	puts("\n\n");
	puts("All Good!");
	puts("\n\n");

	free(pool_ptr);

	return 0; 
}




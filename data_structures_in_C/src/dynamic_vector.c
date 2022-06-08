/**************************************************************
* Author: Gal Dagan
* Date: 28.07.21
* Reviewer: Keren Robins
*
* Description: Implementation of dynamic vector data structure
*
* InfintyLabs OL108
**************************************************************/

#include <stdlib.h> /* malloc, realloc, free */
#include <assert.h> /* assert */
#include <string.h> /* memcpy */ 
#include "dynamic_vector.h"

#define GROWTH_FACTOR 1.5

enum status {SUCCESS, FAILURE, MALLOC_FAIL};

struct vector_t 
{
    void *data;
    size_t capacity;
    size_t vector_size;
    size_t data_size;
};

d_vector_t *VectorCreate(size_t data_size, size_t capacity)
{
	d_vector_t *new_vector = NULL;
	
	assert(capacity > 0);
	
	new_vector = (d_vector_t *)malloc(sizeof(d_vector_t));
		
	if(NULL == new_vector)
	{
		return NULL;
	}
		
	new_vector -> data = malloc(capacity * data_size);	
	if(NULL == new_vector->data)
	{
		free(new_vector);
		return NULL;
	}
		
	new_vector->capacity = capacity;
	new_vector->vector_size = 0;
	new_vector->data_size = data_size;	
	
	return new_vector;
}

void VectorDestroy(d_vector_t *vector)
{
	free(vector -> data);
	vector->data = NULL;
	free(vector);
	vector = NULL;
}

int VectorResize(d_vector_t *vector , size_t new_capacity)
{	
	int status = SUCCESS;
	
	assert(NULL != vector);
	
	if (new_capacity < vector->vector_size)
	{
		status = FAILURE;
	}
	else
	{
		void *temp = realloc(vector->data, (new_capacity + 1) * vector->data_size);
		if (NULL == temp)
		{
			status = MALLOC_FAIL;
		}
		vector->data = temp;
		vector->capacity = new_capacity + 1;
	}
	
	return status;
}

int VectorShrink(d_vector_t *vector)
{
	assert(NULL != vector);
	
	return VectorResize(vector , vector->vector_size);
}

int VectorPushBack(d_vector_t *vector, const void *data)
{
	int status = SUCCESS;
	
	assert(NULL != vector);
	assert(NULL != data);
	
	if (vector->capacity == vector->vector_size)
	{
		status = VectorResize(vector, (vector->capacity) * GROWTH_FACTOR);
	}
	
	if (FAILURE != status)
	{
		memcpy((char *)vector->data + (vector->vector_size * vector->data_size),
				data, vector->data_size);
		++vector->vector_size;
	}

	return status;
}

int VectorPopBack(d_vector_t *vector)
{
	int status = SUCCESS;
	
	assert(NULL != vector);
	
	if (0 == vector->vector_size)
	{
		status = FAILURE;
	}
	else
	{
		--vector->vector_size;
	}
	
	return status;
}

size_t VectorGetSize(const d_vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->vector_size;
}

size_t VectorGetCapacity(const d_vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->capacity;
}

void *VectorGetData(const d_vector_t *vector, int index)
{
	void *runner = NULL;
	
	assert(NULL != vector);
	
	if ((size_t)index < vector->vector_size)
	{
		runner = (char *)vector->data + index * vector->data_size;
	}
	
	return runner;
}

void VectorSetData(d_vector_t *vector, int index, const void *data)
{
	void *runner = NULL;
	
	assert(NULL != vector);
	assert(NULL != data);
	
	runner = (char *)vector->data + index * vector->data_size;
	runner = memcpy(runner, data, vector->data_size);
}


/**********************************************************
* Author: Gal Dagan
* Date: 28.07.21
* Reviewer: Keren Robins
*
* Description: Implementation of stack data structure
*
* InfintyLabs OL108
**********************************************************/

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "stack.h"

struct stack_t
{
	void *bottom;
	void *top;
	size_t capacity;
	size_t data_size;
};

stack_t *StackCreate(size_t capacity, size_t data_size)
{
	stack_t *new_stack = (stack_t *)malloc(sizeof(stack_t));
	
	if (NULL == new_stack)
	{
		return NULL;
	}

	new_stack -> bottom = malloc(data_size * capacity);
	
	if (NULL == new_stack)
	{
		free(new_stack);
		return NULL;
	}
	
	new_stack -> capacity = capacity;
	new_stack -> data_size = data_size; 
	new_stack -> top = (char *)new_stack -> bottom - new_stack -> data_size;
	
	return new_stack;
}

void StackPush(stack_t *stack, const void *data)
{
	assert(NULL != stack);
	
	if (stack -> capacity == StackSize(stack))
	{
		return;
	}
	
	stack -> top = (char *)stack -> top + stack -> data_size;
	stack -> top = memcpy(stack -> top, data, stack -> data_size);
}

void *StackPop(stack_t *stack)
{
	void *result = NULL;
	
	assert(NULL != stack);
	
	if (IsStackEmpty(stack))
	{
		return NULL;
	}
	
	result = stack -> top;
	stack -> top = (char *)stack -> top - stack -> data_size;

	return result;
}

void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	if (IsStackEmpty(stack))
	{
		return NULL;
	}
	
	return stack -> top;
}

size_t StackSize(const stack_t *stack)
{
	size_t byte_diff = 0;
	
	assert(NULL != stack);
	
	if (IsStackEmpty(stack))
	{
		return 0;
	}
	
	byte_diff = (char *)stack -> top - (char *)stack -> bottom;
	
	return byte_diff / stack -> data_size  + 1;	
}

size_t StackCapacity(const stack_t *stack)
{
	assert(NULL != stack);
	
	return stack -> capacity;
}

int IsStackEmpty(const stack_t *stack)
{
	void *where = NULL;
	
	assert(NULL != stack);
	
	where = (char *)stack -> top;
	
	return (char *)where < (char *)stack -> bottom;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	
	free(stack -> bottom);
	free(stack);	
}


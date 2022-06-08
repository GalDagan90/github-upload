/****************************************************************************
*Author : Gal Dagan
*Date : 08.08.21
*Reviewer : Navot Yaari
*
*Description : Implementation of circular buffer
*
*Lab : ol108
****************************************************************************/

#include <string.h>	/* memcpy()			 */
#include <stdlib.h>	/* malloc(), free()  */
#include <assert.h> /* assert()			 */

#include "circular_buffer.h"

#define WORD sizeof(size_t)
#define MIN(a,b) ( ((a) < (b))? (a): (b))

struct circular_buffer
{
	size_t capacity;
	size_t buff_size;
	size_t offset;
	char buff[WORD];
};

/************** static helper functions *************************************/

static void CopyUntillCapacity(cbuff_t *buffer, void *dest);
static void CopyUntillEndIndex(cbuff_t *buffer, void *dest, size_t end_index);
static size_t EndIndexForCopy(cbuff_t *buffer, size_t count);
static size_t StartIndexForWirte(cbuff_t *buffer);

/*****************************************************************************/

cbuff_t *CBuffCreate(size_t capacity)
{
	cbuff_t *new_buff = (cbuff_t *)malloc(sizeof(cbuff_t) - WORD + capacity);
	
	if (NULL == new_buff)
	{
		return NULL;
	}
	
	new_buff->capacity = capacity;
	new_buff->buff_size = 0;
	new_buff->offset = 0;
	
	return new_buff;
}

void CBuffDestroy(cbuff_t *buffer)
{
	assert(NULL != buffer);
	
	free(buffer);
}

size_t CBuffGetFreeSpace(const cbuff_t *buffer)
{
	assert(NULL != buffer);
	
	return buffer->capacity - CBuffGetSize(buffer);
}

size_t CBuffGetSize(const cbuff_t *buffer)
{
	assert(NULL != buffer);
	
	return buffer->buff_size;
}

ssize_t CBuffRead(cbuff_t *buffer, void *dest, size_t count)
{
	size_t end_idx = 0;
	
	assert(NULL != buffer);
	assert(NULL != dest);
	
	if (count > CBuffGetSize(buffer))
	{
		count = buffer->buff_size; 
	}
	
	end_idx = EndIndexForCopy(buffer, count);
	
	if (0 != count)
	{
		if (end_idx > buffer->offset)
		{
			memcpy(dest, buffer->buff + buffer->offset, count);	
		}
		else
		{
			CopyUntillCapacity(buffer, dest);
			CopyUntillEndIndex(buffer, dest, end_idx);
		}	
	}
	
	buffer->offset = end_idx; 
	buffer->buff_size -= count;
	
	return (count);
}

ssize_t CBuffWrite(cbuff_t *buffer, const void *src, size_t count)
{
	size_t write_index = 0;
	size_t loop_counter = count;
	
	assert(NULL != buffer);
	assert(NULL != src);
	
	write_index = StartIndexForWirte(buffer);
	
	for (; loop_counter > 0; --loop_counter, src = (char *)src + 1 )
	{
		memcpy(buffer->buff + write_index, src, 1);
		write_index = (write_index + 1) % buffer->capacity;
	}
	
	/* If overflow has occured */
	if ( count > CBuffGetFreeSpace(buffer))
	{
		buffer->offset = write_index;
	}
	
	buffer->buff_size = MIN(buffer->buff_size + count, buffer->capacity); 
	
	return count;
}

int CBuffIsEmpty(const cbuff_t *buffer)
{
	assert(NULL != buffer);
	
	return (0 == CBuffGetSize(buffer));
}

/**************************************************************************/

static void CopyUntillCapacity(cbuff_t *buffer, void *dest)
{
	size_t bytes_to_copy = buffer->capacity - buffer->offset;
	
	memcpy(dest, buffer->buff + buffer->offset, bytes_to_copy);
}

static void CopyUntillEndIndex(cbuff_t *buffer, void *dest, size_t end_index)
{
	size_t bytes_copied = buffer->capacity - buffer->offset;
	void *dest_runner = (char *)dest + bytes_copied;
	
	memcpy(dest_runner, buffer->buff, end_index);
}

size_t CBuffGetOffset(cbuff_t *buffer)
{
	return buffer->offset;
}

static size_t EndIndexForCopy(cbuff_t *buffer, size_t count)
{
	return (buffer->offset + count) % buffer->capacity;
}

static size_t StartIndexForWirte(cbuff_t *buffer)
{
	return (buffer->offset + CBuffGetSize(buffer)) % buffer->capacity;
}


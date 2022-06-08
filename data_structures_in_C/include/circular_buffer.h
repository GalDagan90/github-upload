/****************************************************************************
*Author : Gal Dagan
*Date : 08.08.21
*Reviewer : Navot Yaari
*
*Description : Implementation of circular buffer
*
*Lab : ol108
****************************************************************************/
#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <sys/types.h> /* ssize_t */
#include <stddef.h> /* ssize_t */

/***************** typdef declaration *****/

typedef struct circular_buffer cbuff_t;

/******************************************/
/**
 * @Description: Creates an empty circular buffer.
 * @Parameters: capacity.
 * @Return: A pointer to the new buffer.
 * @Complexity: O(1).
**/
cbuff_t *CBuffCreate(size_t capacity);

/**
 * @Description: Destroys the circular_buffer.
 * @Parameters: A pointer to the buffer.
 * @Return: void.
 * @Complexity: O(1).
**/
void CBuffDestroy(cbuff_t *buffer);

/**
 * @Description: Returns the number of current free bytes in circular_buffer.
 * @Parameters: A pointer to the buffer.
 * @Return: Num of free bytes in the buffer.
 * @Complexity: O(1).
**/
size_t CBuffGetFreeSpace(const cbuff_t *buffer);

/**
 * @Description: Returns the number of current bytes taken in the buffer.
 * @Parameters: A pointer to the buffer.
 * @Return: Num of bytes currently in the buffer.
 * @Complexity: O(1).
**/
size_t CBuffGetSize(const cbuff_t *buffer);

/**
 * @Description: Reads from the buffer to destination 
 * @Parameters: A pointer to the buffer, A pointer to dest, count - num of
 				bytes to read.
 * @Return: Num of bytes read from the buffer.
 * @Complexity: O(count).
 * @Notes: Will read the min between current buffer size and count.
 *		count cannot be a negative number
**/
ssize_t CBuffRead(cbuff_t *buffer, void *dest, size_t count);

/**
 * @Description: Writes to buffer from source 
 * @Parameters: A pointer to the buffer, A pointer to source, count - num of
 				bytes to read.
 * @Return: Num of bytes written to the buffer from the buffer.
 * @Complexity: O(count).
 * @Notes: Overflow will occur when count given is bigger than num of 
 *		current free bytes. Count cannot be a negative number
**/
ssize_t CBuffWrite(cbuff_t *buffer, const void *src, size_t count);

/**
 * @Description: Check if the buffer is empty .
 * @Parameters: A pointer to the buffer.
 * @Return: One if it is empty, zero if not.
 * @Complexity: O(1).
**/
int CBuffIsEmpty(const cbuff_t *buffer);

/**
 * @Description: Returns the offset index.
 * @Parameters: A pointer to the buffer.
 * @Return: The index of offset.
 * @Complexity: O(1).
 * @Notes: A helper function for testing.
**/
size_t CBuffGetOffset(cbuff_t *buffer);

#endif /* __CIRCULAR_BUFFER_H__ */

/****************************************************************************
* Author: Gal Dagan
* Date: 05.09.21
* Reviewer: Eyal Dafni
*
* Description: Various recursive implementations
*
* InfintyLabs OL108
*****************************************************************************/

#include <assert.h>	/* assert()	*/

#include "recursion.h"

#define NULL_CHAR '\0'

/******************************************************************************
*						Static Function Decleration							  *
******************************************************************************/

static int Strncmp(const char *str1, const char *str2, size_t n);
static void InsertInOrder(stack_t *stack, int val);

/*****************************************************************************/
int FibonacciIter(int element_index)
{
	int result = 1;
	int prev = 0;
	int i = 1;
	
	for (; i < element_index; ++i)
	{
		int temp = result;
		
		result += prev;
		prev = temp;
	}
	
	return ((0 >= element_index) ? 0 : result);
}

int FibonacciRec(int element_index)
{
	if (0 >= element_index)
	{
		return 0;
	}
	else if (1 == element_index)
	{
		return 1;
	}
	
	return FibonacciRec(element_index - 1) + FibonacciRec(element_index - 2);  
}

/*****************************************************************************/

Node *FlipList(Node *node)
{
	Node *flipped = NULL;
	
	assert(NULL != node);
	
	if (NULL == node->next)
	{
		return node;
	}
	
	flipped = FlipList(node->next);
	node->next->next = node;
	node->next = NULL;
	
	return flipped;
}

/*****************************************************************************/

void SortStack(stack_t *stack)
{
	assert(NULL != stack);
	
	if (!IsStackEmpty(stack))
	{
		int popped = *((int *)StackPop(stack));
		SortStack(stack);
		InsertInOrder(stack, popped);
	}
}

static void InsertInOrder(stack_t *stack, int val)
{
	int popped = 0;
	
	if ((IsStackEmpty(stack)) || (val > *((int *)StackPeek(stack))) )
	{
		StackPush(stack, &val);
		
		return;
	}
	
	popped = *((int *)StackPop(stack));
	InsertInOrder(stack, val);
	StackPush(stack, &popped);
}

/*****************************************************************************/

size_t Strlen(const char *str)
{	
	assert(NULL != str);
	
	if (NULL_CHAR == *str)
	{
		return 0;
	}
	
	return (1 + Strlen(str + 1));
}

int Strcmp(const char *str1, const char *str2)
{
	assert(NULL != str1);
	assert(NULL != str2);
	
	if ((NULL_CHAR == *str1) || (*str1 != *str2))
	{
		return (*str1 - *str2);
	}
	
	return (Strcmp(str1 + 1, str2 + 1));
}

char *Strcpy(char *dest, const char *src)
{
	assert(NULL != src);
	assert(NULL != dest);
	
	*dest = *src;
	
	if (NULL_CHAR != *src)
	{
		Strcpy(dest + 1, src + 1);
	}
	
	return dest;	
}

char *Strcat(char *dest, const char *src)
{
	size_t len = 0;
	
	assert(NULL != dest);
	assert(NULL != src);
	
	if (NULL_CHAR == *src)
	{
		return dest;
	}
	
	len = Strlen(dest);
	*(dest + len) = *src; 
	*(dest + len + 1) = NULL_CHAR;
	Strcat(dest + len + 1, src + 1);
	
	return dest;
}

char *Strstr(const char *haystack, const char *needle)
{	
	assert(NULL != haystack);
	assert(NULL != needle);
	
	if (NULL_CHAR == *haystack)
	{
		return NULL;
	}
	else if ( (*haystack == *needle) && 
			  (0 == (Strncmp(haystack, needle, Strlen(needle)))) )
	{
		return ((char *)haystack);
	}
	
	return (Strstr(haystack + 1, needle));
}

static int Strncmp(const char *str1, const char *str2, size_t n)
{
	if (0 == n)
	{
		return 0;
	}
	else if ((NULL_CHAR == *str1) || (*str1 != *str2))
	{
		return (*str1 - *str2);
	}
	
	return (Strncmp(str1 + 1, str2 + 1, n - 1));
}


/**************************************************************
* Author: Gal Dagan
* Date: 05.09.21
* Reviewer: Eyal Dafni
*
* Description: Various recursive implementations
*
* InfintyLabs OL108
**************************************************************/
#ifndef __RECURSION_H_OL108_ILRD__
#define __RECURSION_H_OL108_ILRD__

#include <stddef.h> /* size_t */

#include "stack.h"	/* stack_t, inner stack functions */

typedef struct node
{
	int *data;
	struct node *next;
}Node;

/**
 * @Description: Iteratively calculates the corresponding fib number based on 
 				given index.
 * @Parameters: element_index - index of the fib number to calculate.
 * @Return: Returns the result of the corresponding element_index fib number.
 * @Complexity: O(n) - element_index.
 * @Note: the first number (at index 0) is 0. 
 *		Given a negative number the return value is 0.
**/
int FibonacciIter(int element_index);

/**
 * @Description: Recursively calculates the corresponding fib number based on 
 				given index.
 * @Parameters: element_index - index of the fib number to calculate.
 * @Return: Returns the result of the corresponding element_index fib number.
 * @Complexity: Time: O(2^n) - element_index.
 *				Space O(n) - element_index.
 * @Note: the first number (at index 0) is 0. 
 *		Given a negative number the return value is 0.
**/
int FibonacciRec(int element_index);

/**
 * @Description: Flips a singly linked list in-place.
 * @Parameters: node - a pointer to the start of the list to be reversed.
 * @Return: Returns a pointer to the flipped list.
 * @Complexity: Time: O(n) - list size.
 *				Space: O(n) - list size.
**/
Node *FlipList(Node *node);

/**
 * @Description: Sorts unsorted stack.
 * @Parameters: stack - a pointer to the stack to be sorted.
 * @Return: void.
 * @Complexity: Time: O(n ^ 2) - stack size.
 *				Space: O(n) - stack size.
**/
void SortStack(stack_t *stack);

/**
 * @Description: Calculates the size of string str.
 * @Parameters: str - a pointer to the a string.
 * @Return: size of string.
 * @Complexity: Time: O(n) - string length.
 *				Space: O(n) - string length.
**/
size_t Strlen(const char *str);

/**
 * @Description: Compares lexicographic difference between two strings.
 * @Parameters: str1 - a pointer to the a string.
 *				str2 - a pointer to the a string.
 * @Return: integer that indicates the lexicographic difference between the
 *			two strings.
 * @Complexity: Time: O(n) - shorter string length.
 *				Space: O(n) - shorter string length.
**/
int Strcmp(const char *str1, const char *str2);

/**
 * @Description: Copies the string pointed to by src to the buffer pointed to 
 *				by dest.
 * @Parameters: dest - a pointer to a string to be copied to.
 *				src - a pointer to a string to be copied.
 * @Return: a pointer to the resulting string dest.
 * @Complexity: Time: O(n) - src string length.
 *				Space: O(n) - src string length.
 * @Note: If buffer capacity of dest is too small the behaviour is undefined.
**/
char *Strcpy(char *dest, const char *src);

/**
 * @Description: Appends src string to dest string.
 * @Parameters: dest - a pointer to the a string to append to.
 *				src - a pointer to the a string to be appanded.
 * @Return: a pointer to the resulting string dest.
 * @Complexity: Time: O(n + m) - string lengths.
 *				Space: O(m) - src string length.
 * @Note: If buffer capacity of dest is too small the behaviour is undefined.
**/
char *Strcat(char *dest, const char *src);

/**
 * @Description: Function finds first occurance of the substring needle in the
 *				string haystack.
 * @Parameters: haystack - a pointer to a string to find needle in it.
 *				needle - a pointer to a substring to look for in haystack.
 * @Return: A pointer to the beginning of the located substring, NULL pointer
 *			if substring wasn't found.
 * @Complexity: Time: O(n * m) - string lengths.
 *				Space: O(n) - string length.
**/
char *Strstr(const char *haystack, const char *needle);

#endif /* __RECURSION_H_OL108_ILRD__ */

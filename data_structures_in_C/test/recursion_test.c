/**************************************************************
* Author: Gal Dagan
* Date: 05.09.21
* Reviewer: Eyal Dafni
*
* Description: Various recursive implementations
*
* InfintyLabs OL108
**************************************************************/

#include <stdio.h> 	/* printf() */
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* strlen(), strcmp() */
#include <assert.h>	/* assert() */

#include "recursion.h"

static void TestFibo();
static void TestFlipped();
static void TestString();
static void TestSortStack();

static Node *AddToList(Node *head, int *data);
static Node *MakeNode(int *data);

int main()
{
	TestFibo();
	TestFlipped();
	TestString();
	TestSortStack();
	
	printf("\n");
	printf("Passed!\n");
	printf("\n");
	
	return (0);
}

static void TestFibo()
{
	assert(FibonacciIter(9) == FibonacciRec(9));
	assert(FibonacciIter(0) == FibonacciRec(0));
	assert(FibonacciIter(1) == FibonacciRec(1));
	assert(FibonacciIter(-1) == FibonacciRec(-1));
}

static void TestFlipped()
{
	Node *head = NULL;
	int arr[] = {1,2,3,4,5};
	int i = 0;
	
	head = MakeNode(&arr[0]);
	head = AddToList(head, &arr[1]);
	head = AddToList(head, &arr[2]);
	head = AddToList(head, &arr[3]);
	head = AddToList(head, &arr[4]);
	
	assert(*(head->data) == 5 && *(head->next->data) == 4);
	head = FlipList(head);
	assert(*(head->data) == 1 && *(head->next->data) == 2);
	
	for (; i < 5; ++i)
	{
		Node *temp = head->next;
		free(head);
		head = temp;
	}
	
}

static void TestString()
{
	char str1[100] = "this string has 24 chars";
	char str2[100] = "THIS string is for copy";
	char *str3 = NULL;
	
	assert(strlen(str1) == Strlen(str1));
	assert(strlen(str2) == Strlen(str2));
	
	Strcpy(str1, str2);
	assert(strcmp(str1, str2) == 0);
	
	Strcat(str1, " now i used strcat");
	printf("%s\n", str1);
	assert(strcmp(str1, "THIS string is for copy now i used strcat") == 0);
	
	str3 = Strstr(str1, "str");
	assert(0 == strcmp(str3, "string is for copy now i used strcat"));
	str3 = Strstr(str1, "Not in string");
	assert(NULL == str3);
}

static void TestSortStack()
{
	stack_t *stack = StackCreate(7, sizeof(int));
	int arr[] = {47,9,-5,76,7,3,12};
	int i = 0;
	int top = 0;
	
	for (i = 0; i < 7; ++i)
	{
		StackPush(stack, &arr[i]);
	}
	
	assert(StackSize(stack) == 7);
	assert(12 == *((int *)StackPeek(stack)));
	
	SortStack(stack);
	top = *((int *)StackPop(stack));
	for (; !IsStackEmpty(stack) ; top = *((int *)StackPop(stack)) )
	{
		assert(top >= *((int *)StackPeek(stack)) );
	}
	
	StackDestroy(stack);
}

static Node *AddToList(Node *head, int *data)
{	
	Node *new_node = MakeNode(data);
	
	new_node->next = head;
	head = new_node;
	
	return head;
}

static Node *MakeNode(int *data)
{
	Node *node = (Node *)malloc(sizeof(Node));
	node->data = data;
	node->next = NULL;
	
	return node;
}



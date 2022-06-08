#include <stdio.h>  /* printf */
#include "stack.h"

#define TEST_ARR_SIZE 5

void TestStackCreate(stack_t *stack);
void TestIsStackEmpty(stack_t *stack);
void TestStackSize(stack_t *stack);
void TestStackPeek(stack_t *stack, int *arr);
void TestStackPop(stack_t *stack);
void TestStackPush(stack_t *stack, int *arr);
void TestStackCapacity(stack_t *stack);

int main()
{
	stack_t *stack = NULL;
	int arr_data[TEST_ARR_SIZE] = {-5, 4, 893, 1056, -485};
	size_t i = 0;
	
	stack = StackCreate(TEST_ARR_SIZE, sizeof(int));
	
	TestStackCreate(stack);
	TestIsStackEmpty(stack);
	
	for (; i < TEST_ARR_SIZE; ++i)
	{
		StackPush(stack, &arr_data[i]);
	}
	
	TestStackSize(stack);
	TestStackPeek(stack, arr_data);
	TestStackPop(stack);
	TestStackPush(stack, arr_data);
	TestStackCapacity(stack);
	StackDestroy(stack);
	
	return 0;
}

void TestStackCreate(stack_t *stack)
{
	char str[10] = {'\0'};
	strcpy(str, (NULL == stack)? "FAILURE": "SUCCESS");
	printf("Stack Create: %s\n", str);
}

void TestIsStackEmpty(stack_t *stack)
{
	char str[10] = {'\0'};

	if (0 == StackSize(stack) && IsStackEmpty(stack))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	printf("IsStackEmpty: %s\n", str);
}

void TestStackSize(stack_t *stack)
{
	char str[10] = {'\0'};
	
	if ((size_t)StackSize(stack) == TEST_ARR_SIZE)
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	printf("StackSize: %s\n", str);
}

void TestStackPeek(stack_t *stack, int *arr)
{
	char str[10] = {'\0'};
	
	if (arr[TEST_ARR_SIZE - 1] == (*(int *)StackPeek(stack)))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	printf("Stack Peek: %s\n", str);
}

void TestStackPop(stack_t *stack)
{
	char str[10] = {'\0'};
	StackPop(stack);
	StackPop(stack);
	if ((size_t)StackSize(stack) == TEST_ARR_SIZE - 2)
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	printf("StackPop: %s\n", str);
}

void TestStackPush(stack_t *stack, int *arr)
{
	char str[10] = {'\0'};
	StackPush(stack, &arr[TEST_ARR_SIZE - 1]);
	if (arr[TEST_ARR_SIZE - 1] == (*(int *)StackPeek(stack)))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	printf("StackPush: %s\n", str);
}

void TestStackCapacity(stack_t *stack)
{
	char str[10] = {'\0'};
	if (TEST_ARR_SIZE == StackCapacity(stack))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	printf("StackCapacity: %s\n", str);
}

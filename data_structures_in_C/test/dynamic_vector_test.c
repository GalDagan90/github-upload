#include <stdio.h> /* printf */
#include <string.h> /* strcpy */

#include "dynamic_vector.h"

#define GROWTH_FACTOR 1.5
#define TEST_ARR_SIZE 6
#define INITIAL_CAPACITY 10
#define RESIZE_SIZE 20

void TestVectorCreate(d_vector_t *vector);
void TestVectorGetSize(d_vector_t *vector);
void TestVectorGetCapacity(d_vector_t *vector);
void TestVectorPushBack(d_vector_t *vector, int *num);
void TestVectorPopBack(d_vector_t *vector);
void TestVectorGetData(d_vector_t *vector, int *arr);
void TestVectorSetData(d_vector_t *vector, int *num);
void TestVectorResize(d_vector_t *vector);
void TestVectorShrink(d_vector_t *vector);

int main()
{
	int arr[TEST_ARR_SIZE] = {54, -89, 10235, 587, 174, -10};
	d_vector_t *vector = NULL;
	int x = 100;
	int i = 0;
	
	vector = VectorCreate(sizeof(int), INITIAL_CAPACITY); 
	
	for (; i < TEST_ARR_SIZE; ++i)
	{
		VectorPushBack(vector, &arr[i]);
	}
	
	TestVectorCreate(vector);
	TestVectorGetSize(vector);
	TestVectorGetCapacity(vector);
	TestVectorPushBack(vector, &x);
	TestVectorPopBack(vector);
	TestVectorGetData(vector, arr);
	TestVectorSetData(vector, &x);
	TestVectorResize(vector);
	TestVectorShrink(vector);
	
	VectorDestroy(vector);

	return (0);
}

void TestVectorCreate(d_vector_t *vector)
{
	char str[10] = {'\0'};
	
	strcpy(str, (NULL == vector)? "FAILURE": "SUCCESS");
	printf("Vector Create: %s\n", str);
}

void TestVectorGetSize(d_vector_t *vector)
{
	char str[10] = {'\0'};
	
	if (TEST_ARR_SIZE == VectorGetSize(vector))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorGetSize: %s\n", str);
}

void TestVectorGetCapacity(d_vector_t *vector)
{
	char str[10] = {'\0'};
	
	if (INITIAL_CAPACITY == VectorGetCapacity(vector))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorGetCapacity: %s\n", str);
}

void TestVectorPushBack(d_vector_t *vector, int *num)
{
	char str[10] = {'\0'};
	
	VectorPushBack(vector, num);
	
	if (TEST_ARR_SIZE + 1 == VectorGetSize(vector))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorPushBack: %s\n", str);
}

void TestVectorPopBack(d_vector_t *vector)
{
	char str[10] = {'\0'};
	
	VectorPopBack(vector);
	if (TEST_ARR_SIZE == VectorGetSize(vector))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorPopBack: %s\n", str);
}

void TestVectorGetData(d_vector_t *vector, int *arr)
{
	char str[10] = {'\0'};
	int num = *(int *)VectorGetData(vector, 3);
	
	if (arr[3] == num)
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorGetData: %s\n", str);
}

void TestVectorSetData(d_vector_t *vector, int *num)
{
	char str[10] = {'\0'};
	int temp = 0;
	
	VectorSetData(vector, 3, num);
	temp = *(int *)VectorGetData(vector, 3);
	
	if (temp == *num)
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorSetData: %s\n", str);
}

void TestVectorResize(d_vector_t *vector)
{
	char str[10] = {'\0'};
	
	VectorResize(vector, RESIZE_SIZE);
	
	if (RESIZE_SIZE == VectorGetCapacity(vector))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorResize: %s\n", str);
}

void TestVectorShrink(d_vector_t *vector)
{
	char str[10] = {'\0'};
	
	VectorShrink(vector);
	
	if (VectorGetSize(vector) == VectorGetCapacity(vector))
	{
		strcpy(str, "SUCCESS");
	}
	else
	{
		strcpy(str, "FAILURE");
	}
	
	printf("VectorShrink: %s\n", str);
}

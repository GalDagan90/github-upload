/****************************************************************************
*Author : Gal Dagan
*Date : 10.09.21
*Reviewer :	Liel Sananes
*
*Description : Implementation of AVL trees with recursive functions
*
*Lab : ol108
****************************************************************************/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "avl.h"

#define UNUSED(x) (void)(x)
#define ARR_SIZE 100

int CompareFunction(const void *lhs, const void *rhs);

static void TestRandTree();
static void TestNonRandTree();
static void InitArr(int *arr);
static void TestAVLFind(avl_t *avl, int *arr);


int main(void)
{
	TestRandTree();
	TestNonRandTree();

	return (0);
}

int CompareFunction(const void *lhs, const void *rhs)
{
	return (*(int *)lhs - *(int *)rhs);
}

static void TestRandTree()
{
	avl_cmp_t cmp_f = &CompareFunction;
	avl_t *avl = AVLCreate(cmp_f);
	int rand_arr[ARR_SIZE] = {0};
	int i = 0;
	
	assert(NULL != avl);
	assert(AVLIsEmpty(avl));
	
	InitArr(rand_arr);
	
	for (; i < ARR_SIZE; ++i)
	{
		AVLInsert(avl, &rand_arr[i]);
	}
	
	assert(100 == AVLSize(avl));
	TestAVLFind(avl, rand_arr);
	
	AVLDestroy(avl);
	
	printf("Random tree test completed\n");
}

static void InitArr(int *arr)
{
	int i = 0;
	
	for (; i < ARR_SIZE; ++i)
	{
		arr[i] = rand() % 1000;
	}
}

static void TestAVLFind(avl_t *avl, int *arr)
{
	void *data = NULL;
	int i = 9999;
	
	data = AVLFind(avl, &arr[13]);
	assert(*(int *)data == arr[13]);
	AVLRemove(avl, data);
	assert(AVLSize(avl) == 99);
	
	data = AVLFind(avl, &arr[27]);
	assert(*(int *)data == arr[27]);
	AVLRemove(avl, data);
	assert(AVLSize(avl) == 98);
	
	data = AVLFind(avl, &arr[73]);
	assert(*(int *)data == arr[73]);
	AVLRemove(avl, data);
	assert(AVLSize(avl) == 97);
	
	data = AVLFind(avl, &arr[99]);
	assert(*(int *)data == arr[99]);
	data = AVLFind(avl, &i);
	assert(data == NULL);
}

static void TestNonRandTree()
{
	avl_cmp_t cmp_f = &CompareFunction;
	int arr[] = {383, 886, 777,1000,899,200,250,275,400};
	int i = 0;
	avl_t *avl = AVLCreate(cmp_f);
	
	for (; i < 9; ++i)
	{
		AVLInsert(avl, &arr[i]);
	}
	assert(9 == AVLSize(avl));
	AVLRemove(avl, &arr[5]); /* delete 200 */
	AVLRemove(avl, &arr[7]); /* delete 275 (leaf node) */
	assert(7 == AVLSize(avl));
	AVLDestroy(avl);
	printf("None-Random tree test completed\n");
}







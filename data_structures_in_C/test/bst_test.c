/**************************************************************
* Author: Gal Dagan
* Date: 30.08.21
* Reviewer: Aharon Luzon
*
* Description: Implementation of Binary Search Tree (BST)
*
* InfintyLabs OL108
**************************************************************/

#include <stdio.h> 	/* printf() */
#include <assert.h>	/* assert() */

#include "bst.h"

#define ARR_SIZE 16

int CompareFunction(const void *lhs, const void *rhs);

static void TestBSTBegin(bst_t *bst, int *arr);
static void TestBSTNext(bst_t *bst);
static void TestBSTPrev(bst_t *bst);
static void TestBSTFind(bst_t *bst);
static void TestBSTSize(bst_t *bst, size_t size);

int main(void)
{
	bst_t *bst = NULL;
	bst_cmp_t bst_cmp_func  = &CompareFunction;
	
	int arr[] = {15,5,40,3,12,20,50,1,4,10,14,18,100,11,16,19};
	int i = 0;
	
	bst = BSTCreate(bst_cmp_func);
	assert(NULL != bst);
	printf("BSTCreate\n");
	assert(BSTIsEmpty(bst));
	printf("BSTIsEmpty\n");
	
	for (; i < ARR_SIZE; ++i)
	{
		BSTInsert(bst, &arr[i]);
	}
	printf("BSTInsert\n");
	assert(!BSTIsEmpty(bst));
	
	TestBSTSize(bst, ARR_SIZE);
	TestBSTBegin(bst, arr);
	TestBSTNext(bst);
	TestBSTPrev(bst);
	TestBSTFind(bst);
	
	BSTDestroy(bst);

	return (0);
}

int CompareFunction(const void *lhs, const void *rhs)
{
	return (*(int *)lhs - *(int *)rhs);
}

static void TestBSTSize(bst_t *bst, size_t size)
{
	assert(BSTSize(bst) == size);
	printf("BSTSize\n");
	printf("BSTForEach\n");
}

static void TestBSTBegin(bst_t *bst, int *arr)
{
	bst_iter_t iter = BSTBegin(bst);
	assert(*(int *)BSTGetData(iter) == arr[7]);
	printf("BSTBegin\n");
}

static void TestBSTNext(bst_t *bst)
{
	bst_iter_t iter = BSTBegin(bst);
	int arr[7] = {1,3,4,5,10,11,12};
	int i = 1;
	
	for (; i < 7; ++i)
	{
		iter = BSTNext(iter);
		assert(*(int *)BSTGetData(iter) == arr[i]);
	}
	printf("BSTNext\n");
	
}

static void TestBSTPrev(bst_t *bst)
{
	bst_iter_t iter = BSTEnd(bst);
	int arr[8] = {100,50,40,20,19,18,16,15};
	int i = 0;
	
	for (; i < 8; ++i)
	{
		iter = BSTPrev(iter);
		assert(*(int *)BSTGetData(iter) == arr[i]);
	}
	printf("BSTBegin\n");
	printf("BSTPrev\n");
}

static void TestBSTFind(bst_t *bst)
{
	bst_iter_t iter = {NULL};
	int w = 200;
	int x = 16;
	int y = 18;
	int z = 40;
	
	iter = BSTFind(bst, &w);
	assert(BSTGetData(iter) == NULL);
	iter = BSTFind(bst, &x);
	assert(*(int *)BSTGetData(iter) == x);
	BSTRemove(iter);
	iter = BSTFind(bst, &y);
	assert(*(int *)BSTGetData(iter) == y);
	BSTRemove(iter);
	iter = BSTFind(bst, &z);
	assert(*(int *)BSTGetData(iter) == z);
	BSTRemove(iter);
	printf("BSTFind\n");
	printf("BSTRemove\n");
	TestBSTSize(bst, ARR_SIZE - 3);
}




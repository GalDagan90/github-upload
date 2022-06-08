/**************************************************************
* Author: Gal Dagan
* Date: 12.08.21
* Reviewer: Eyal Dafni 
*
* Description: Implementation of Priority queue data structure
*
* InfintyLabs OL108
**************************************************************/

#include <stdio.h> 	/* printf() */
#include <assert.h>	/* assert() */

#include "priority_queue.h"

#define ARR_SIZE 5

int CompareFunction(const void *lhs, const void *rhs);
int IsSame(const void *lhs, const void *rhs);
static void TestCreate(pq_t *pq);
static void TestSize(pq_t *pq);
static void TestIsEmpty(pq_t *pq);
static void TestPQEnqueue(pq_t *pq, int *arr);
static void TestPQPeek(pq_t *pq, int *arr);
static void TestPQDequeue(pq_t *pq);
static void TestPQClear(pq_t *pq);
static void TestPQErase(pq_t *pq, int *arr, is_match_pq_t is_match);

int main(void)
{
	pq_t *pqueue = NULL;
	pq_cmp_priority_t cmp_p_t = &CompareFunction;
	is_match_pq_t is_match = &IsSame;
	int arr[ARR_SIZE] = {100, 50, 60, 47, 9};
	
	pqueue = PQCreate(cmp_p_t);
	TestCreate(pqueue);
	TestSize(pqueue);
	TestIsEmpty(pqueue);
	TestPQEnqueue(pqueue, arr);
	TestPQPeek(pqueue, arr);
	TestPQDequeue(pqueue);
	TestPQClear(pqueue);
	TestPQEnqueue(pqueue, arr);
	TestPQErase(pqueue, arr, is_match);
	PQDestroy(pqueue);
	
	return (0);
}

int CompareFunction(const void *lhs, const void *rhs)
{
	return (*(int *)rhs - *(int *)lhs);
}

int IsSame(const void *lhs, const void *rhs)
{
	return (*(int *)lhs == *(int *)rhs);
}

static void TestCreate(pq_t *pq)
{
	printf("TestCreate\n");
	assert(NULL != pq);
}

static void TestSize(pq_t *pq)
{	
	size_t size = PQSize(pq);
	
	printf("TestCreate\n");
	assert(size == 0 || size == ARR_SIZE);
}

static void TestIsEmpty(pq_t *pq)
{	
	printf("TestIsEmpty\n");
	assert(PQIsEmpty(pq) == 0 || PQIsEmpty(pq) == 1);
}

static void TestPQEnqueue(pq_t *pq, int *arr)
{
	int i = 0;
	printf("TestPQEnqueue\n");
	for (; i < ARR_SIZE; ++i)
	{
		PQEnqueue(pq, &arr[i]);
	}
	
	assert(PQSize(pq) == ARR_SIZE);
}

static void TestPQPeek(pq_t *pq, int *arr)
{
	printf("TestPQPeek\n");
	assert(*(int *)PQPeek(pq) == arr[0]);
}

static void TestPQDequeue(pq_t *pq)
{
	void *data = PQPeek(pq);
	
	printf("TestPQDEqueue\n");
	assert(*(int *)data == *(int *)PQDequeue(pq));
	assert(PQSize(pq) == ARR_SIZE - 1);
}

static void TestPQClear(pq_t *pq)
{
	printf("TestPQClear\n");
	PQClear(pq);
	assert(PQSize(pq) == 0);
}

static void TestPQErase(pq_t *pq, int *arr, is_match_pq_t is_match)
{
	int x = 30;
	
	printf("TestPQErase\n");
	assert(*(int *)PQErase(pq, &arr[1], is_match) == arr[1]);
	assert(PQSize(pq) == ARR_SIZE - 1);
	printf("TestPQErase\n");
	assert(PQErase(pq, &x, is_match) == NULL);
	assert(PQSize(pq) == ARR_SIZE - 1);
	printf("TestPQErase\n");
	PQErase(pq, &arr[0], is_match);
	assert(PQSize(pq) == ARR_SIZE - 2);
	printf("TestPQErase\n");
	PQErase(pq, &arr[0], is_match);
	assert(PQSize(pq) == ARR_SIZE - 2);
}

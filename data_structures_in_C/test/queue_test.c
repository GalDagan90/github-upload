#include <stdio.h> 	/* printf() */
#include <assert.h>	/* assert() */

#include "queue.h"
#include "singly_linked_list.h"

#define ARR_SIZE 10

enum status
{
	SUCCESS = 0,
	FAILURE
};

static void PrintTestResult(int status);
static void TestQueueCreate(queue_t *queue);
static void TestIsQueueEmpty(queue_t *queue, size_t num_of_elements);
static void TestQueueGetSize(queue_t *queue, size_t num_of_elements);
static void TestQueueEnqueue(queue_t *queue, int *arr);
static void TestQueueDequeue(queue_t *queue, int times_dequed);
static void TestQueuePeek(queue_t *queue, int *arr, int times_dequed);

int main()
{
	int arr[ARR_SIZE] = {1, 5, -56, 24, -245, 963, 10, 555, -444, 666};
	queue_t *queue = NULL;
	
	queue = QueueCreate();
	
	/* Before Insertion */
	TestQueueCreate(queue);
	TestIsQueueEmpty(queue, 0);
	TestQueueGetSize(queue, 0);
	
	/* Insertion */
	TestQueueEnqueue(queue, arr);
	TestQueueGetSize(queue, ARR_SIZE);
	TestQueueDequeue(queue, 1);
	TestQueueDequeue(queue, 2);
	TestQueueDequeue(queue, 3);
	TestQueuePeek(queue, arr, 3);
	
	QueueDestroy(queue);

	return (0);
}


static void PrintTestResult(int status)
{
	if (SUCCESS == status)
	{
		printf("SUCCESS\n");
	}
	else
	{
		printf("FAILURE\n");
	}
}

static void TestQueueCreate(queue_t *queue)
{
	printf("Queue Create Test:\t");
	
	(NULL != queue)? PrintTestResult(SUCCESS): PrintTestResult(FAILURE);
}

static void TestIsQueueEmpty(queue_t *queue, size_t num_of_elements)
{
	int status = SUCCESS;
	
	printf("Queue IsEmpty Test:\t");
	if (QueueIsEmpty(queue) && 0 == num_of_elements)
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestQueueGetSize(queue_t *queue, size_t num_of_elements)
{
	int status = SUCCESS;
	
	printf("Queue GetSize Test:\t");
	if (QueueGetSize(queue) == num_of_elements)
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestQueueEnqueue(queue_t *queue, int *arr)
{
	int status = SUCCESS;
	size_t runner = 0;
	
	printf("Enqueue Test:\t\t");
	
	for (; runner < ARR_SIZE; ++runner)
	{
		status = QueueEnqueue(queue, &arr[runner]);
	}
	
	PrintTestResult(status);
}

static void TestQueueDequeue(queue_t *queue, int times_dequed)
{
	int status = SUCCESS;
	
	printf("Dequeue Test:\t\t");
	QueueDequeue(queue);
	
	if ((size_t)(ARR_SIZE - times_dequed) == QueueGetSize(queue))
	{
		status = SUCCESS;
	}
	else
	{
		status = FAILURE;
	}
	
	PrintTestResult(status);
}

static void TestQueuePeek(queue_t *queue, int *arr, int times_dequed)
{
	int status = SUCCESS;
	
	printf("QueuePeek Test:\t\t");
	status = (*(int *)QueuePeek(queue) == arr[times_dequed])? SUCCESS: FAILURE;
	PrintTestResult(status);
}


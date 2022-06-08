#include <assert.h> /* assert() */
#include <unistd.h>	/* getpid() */
#include <stdio.h>	/* printf() */

#include "task.h"

typedef struct test_args
{
	char *string;
	int counter;
}test_t;

static void TaskTest(test_t test);
int Operation(void *args);
void CleanUp(void *args);

int main(void)
{
	test_t test1;
	
	test1.string = "Hello";
	test1.counter = 3;
	
	TaskTest(test1);
			
	return (0);
}

static void TaskTest(test_t test)
{
	task_t *task = NULL;
	task_operation_func_t operation = &Operation;
	time_t start_time = time(0) + 10;
	time_t time_interval = 30;
	task_cleanup_func_t cleanup = &CleanUp;
	
	task = TaskCreate(operation, start_time, time_interval, &test, cleanup);
	
	assert(NULL != task);
	assert(!IsSameUuid(bad_uuid, TaskGetUID(task)));
	assert(start_time == TaskGetStartTime(task));
	TaskRunOperation(task);
	assert(2 == test.counter);
	TaskUpdateStartTime(task);
	assert(start_time + time_interval == TaskGetStartTime(task));
}

int Operation(void *args)
{
	test_t *test = (test_t *)args;
	
	printf("%s\n", test->string);
	--test->counter;
	
	return (test->counter);
}

void CleanUp(void *args)
{
	test_t *test = (test_t *)args;
	
	test->counter = 0;
}


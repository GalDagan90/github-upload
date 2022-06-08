/**************************************************************
* Author: Gal Dagan
* Date: 16.08.21
* Reviewer: Navot Yaari
*
* Description: Implementation of Tasker
*
* InfintyLabs OL108
**************************************************************/

#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() 		*/

#include "task.h"

struct task
{
	uuid_t uid;
	time_t start_time;
	time_t time_interval;
	task_operation_func_t operation;
	task_cleanup_func_t cleanup;
	void *args;
};

task_t *TaskCreate(task_operation_func_t operation, time_t start_time
		, time_t time_interval, void *args, task_cleanup_func_t cleanup)
{
	task_t *new_task = (task_t *)malloc(sizeof(task_t));
	if (NULL == new_task)
	{
		return NULL;
	}
	
	new_task->uid = UuidCreate();
	if (IsSameUuid(new_task->uid, bad_uuid))
	{
		free(new_task);
		new_task = NULL;
		return NULL;
	}
	
	new_task->start_time = start_time;
	new_task->time_interval = time_interval;
	new_task->args = args;
	new_task->operation = operation;
	new_task->cleanup = cleanup;
	
	return new_task;
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	
	task->cleanup(task->args);
	free(task);
	task = NULL;
}

uuid_t TaskGetUID(const task_t *task)
{
	assert(NULL != task);
	
	return (task->uid);
}

time_t TaskGetStartTime(const task_t *task)
{
	assert(NULL != task);
	
	return (task->start_time);
}

int TaskRunOperation(const task_t *task)
{
	assert(NULL != task);
	
	return (task->operation(task->args));
}

task_t *TaskUpdateStartTime(task_t *task)
{
	assert(NULL != task);
	
	task->start_time += task->time_interval;
	
	return task;
}



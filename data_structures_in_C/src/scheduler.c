/**************************************************************
* Author: Gal Dagan
* Date: 16.08.21
* Reviewer: Navot Yaari
*
* Description: Implementation of a simple scheduler
*
* InfintyLabs OL108
**************************************************************/
#include <stdlib.h> /* malloc(), free() */
#include <assert.h> /* assert() 		*/
#include <unistd.h>	/* sleep()			*/

#include "scheduler.h"
#include "task.h"
#include "priority_queue.h"


struct scheduler
{
	pq_t *pq;
	int stop_flag;
};

enum status
{
	SUCCESS = 0
};

int StartTimeCmp(const void *task1, const void *task2);
int IsSameTaskUID(const void *task, const void *uuid);

sched_t *SchedCreate(void)
{
	sched_t *new_scheduler = (sched_t *)malloc(sizeof(sched_t));
	if (NULL == new_scheduler)
	{
		return NULL;
	}
	
	new_scheduler->pq= PQCreate(StartTimeCmp);
	if (NULL == new_scheduler->pq)
	{
		free(new_scheduler);
		new_scheduler = NULL;
		return NULL;
	}
	
	new_scheduler->stop_flag = 0;
	
	return new_scheduler;
}

void SchedDestroy(sched_t *sched)
{
	assert(NULL != sched);
	
	SchedClear(sched);
	PQDestroy(sched->pq);
	free(sched);
	sched = NULL;
}

uuid_t SchedAddTask(sched_t *sched, sched_operation_func_t operation_func,
					time_t start_time, time_t time_interval, void *args,
					sched_cleanup_func_t cleanup_func)
{
	task_t *new_task = NULL;
	
	new_task = TaskCreate(operation_func, start_time, time_interval, args,
		 					cleanup_func);
	if (NULL == new_task)
	{
		return bad_uuid;
	}
	
	if (SUCCESS != PQEnqueue(sched->pq, new_task))
	{
		free(new_task);
		return bad_uuid;
	}
	
	return TaskGetUID(new_task);
}

int SchedRemoveTask(sched_t *sched, uuid_t uuid)
{
	task_t *task = NULL;
	
	assert(NULL != sched);
	
	task = (task_t *)(PQErase(sched->pq, &uuid, IsSameTaskUID));
	if (NULL != task)
	{
		TaskDestroy(task);
	}
	
	return (NULL == task);
}

void SchedRun(sched_t *sched)
{
	assert(NULL != sched);
	
	while (0 == sched->stop_flag && !SchedIsEmpty(sched))
	{
		int status = SUCCESS;
		time_t curr_time = time(NULL);
		task_t *task = (task_t *)(PQDequeue(sched->pq));
		
		if (curr_time < TaskGetStartTime(task))
		{
			sleep(TaskGetStartTime(task) - curr_time);
		}
		
		status = TaskRunOperation(task);
		if (SUCCESS == status)
		{
			TaskUpdateStartTime(task);
			PQEnqueue(sched->pq, task);			
		}
		else
		{
			TaskDestroy(task);
		}
	}
}

int SchedStop(sched_t *sched)
{
	assert(NULL != sched);
	
	sched->stop_flag = 1;
	
	return (sched->stop_flag);
}

int SchedIsEmpty(const sched_t *sched)
{
	assert(NULL != sched);
	
	return PQIsEmpty(sched->pq);
}

size_t SchedSize(const sched_t *sched)
{
	assert(NULL != sched);
	
	return PQSize(sched->pq);
}

void SchedClear(sched_t *sched)
{
	assert(NULL != sched);
	
	while (!SchedIsEmpty(sched))
	{
		TaskDestroy(PQDequeue(sched->pq));
	}
	
	PQClear(sched->pq);
}

int StartTimeCmp(const void *task1, const void *task2)
{
	time_t time1 = 0;
	time_t time2 = 0;
	
	assert(NULL != task1);
	assert(NULL != task2);
	
	time1 = TaskGetStartTime((task_t *)(task1));
	time2 = TaskGetStartTime((task_t *)(task2));
	
	return (int)(time1 - time2);
}

int IsSameTaskUID(const void *task, const void *uuid)
{
	uuid_t task_uuid = {0,0,0};
	
	assert(NULL != task);
	assert(NULL != uuid);
	
	task_uuid = TaskGetUID((task_t *)(task));
	return IsSameUuid(task_uuid, *(uuid_t *)uuid);
}

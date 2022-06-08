/**************************************************************
* Author: Gal Dagan
* Date: 16.08.21
* Reviewer: Navot Yaari
*
* Description: Implementation of Tasker
*
* InfintyLabs OL108
**************************************************************/
#ifndef __TASK_H_OL108_ILRD__
#define __TASK_H_OL108_ILRD__

#include <time.h>
#include <stddef.h>

#include "uid.h"

typedef struct task task_t;

/**
 * @Description: Function pointer type that performs an operation on a task.
 *				returns 0 whether the task should run again, non-zero otherwise.
 * @Parameters: A void pointers to element.
 * @Return: returns 0 if the operation should run again, non-zero otherwise.
**/
typedef int (* task_operation_func_t) (void *);

/**
 * @Description: Function pointer type that performs cleanup after all tasks 
 *				are done.
 * @Parameters: A void pointer to element. 								
 * @Return: returns 0 upon SUCCESS, non-zero otherwise.
**/
typedef void (* task_cleanup_func_t) (void *);

/******************************************************************************
 *                           FUNCTION DECLARATIONS                            *
 ******************************************************************************/

/**
 * @Description: Create a task with assosiated operation and cleanup functions.
 * @Parameters: operation - pointer to an operation function.
 *				start_time - start time for operation.
 *				time_interval - interval between two task operations
 *				args - pointer to argument for the operation and cleanup function.
 *				cleanup - pointer to a cleanup function
 * @Return: A pointer to the new priority queue.
 * @Complexity: O(1).
**/
task_t *TaskCreate(task_operation_func_t operation, time_t start_time
		, time_t time_interval, void *args, task_cleanup_func_t cleanup);

/**
 * @Description: Performs a clean-up on a task.
 * @Parameters: task - a pointer to the task.
 * @Return: void.
 * @Complexity: Depands on the cleanup function.
**/
void TaskDestroy(task_t *task);

/**
 * @Description: Return uid struct from a task.
 * @Parameters: task - a pointer to the task.
 * @Return: start time of a given stack.
 * @Complexity: O(1).
**/
uuid_t TaskGetUID(const task_t *task);

/**
 * @Description: Return the start time of a task.
 * @Parameters: task - a pointer to the task.
 * @Return: start time of a given stack.
 * @Complexity: O(1).
**/
time_t TaskGetStartTime(const task_t *task);

/**
 * @Description: Run the task.
 * @Parameters: task - a pointer to the task.
 * @Return: 0 if the task should be repeated in the futer, non-zero otherwise.
 * @Complexity: Depends on the operation func
**/
int TaskRunOperation(const task_t *task);

/**
 * @Description: Updates the start time of a task.
 * @Parameters: task - a pointer to the task.
 * @Return: a pointer to the task.
 * @Complexity: O(1).
**/
task_t *TaskUpdateStartTime(task_t *task);

#endif /* __TASK_H_OL108_ILRD__ */

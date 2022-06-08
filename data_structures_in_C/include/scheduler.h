/**************************************************************
* Author: Gal Dagan
* Date: 16.08.21
* Reviewer: Navot Yaari
*
* Description: Implementation of a simple scheduler
*
* InfintyLabs OL108
**************************************************************/
#ifndef __SCHEDULER_H_OL108_ILRD__
#define __SCHEDULER_H_OL108_ILRD__

#include <time.h> 	/* time_t, time() 	*/
#include <stddef.h>	/* size_t			*/

#include "uid.h"

/**
 * @Description: Function pointer type that performs an operation on a task.
 *				returns whether the task should be run again or not.
 * @Parameters: A void pointers to element.
 * @Return: returns 0 if the operation should run again, non-zero otherwise.
**/
typedef int (*sched_operation_func_t) (void *);

/**
 * @Description: Function pointer type that performs cleanup after all operation
 *				are done.
 * @Parameters: A void pointers to element.
 * @Return: returns 0 upon SUCCESS, non-zero otherwise.
**/
typedef void (*sched_cleanup_func_t) (void *);


typedef struct scheduler sched_t;

/******************************************************************************
 *                           FUNCTION DECLARATIONS                            *
 ******************************************************************************/

/**
 * @Description: Create an empty scheduler.
 * @Parameters: void.
 * @Return: A pointer to the new scheduler.
 * @Complexity: O(1).
**/
sched_t *SchedCreate(void);

/**
 * @Description: Destroy the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 * @Return: void.
 * @Complexity: Depends on the complexity of the cleanup function.
**/
void SchedDestroy(sched_t *sched);

/**
 * @Description: Add a new task to the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 *				operation - pointer to an operation function.
 *				start_time - start time for operation.
 *				time_interval - interval between two task operations
 *				args - pointer to argument for the operation and cleanup function.
 *				cleanup - pointer to a cleanup function
 * @Return: uuid of the task added.
 * @Complexity: O(n) (n = number of elements in the scheduler).
**/
uuid_t SchedAddTask(sched_t *sched, sched_operation_func_t operation_func, 
					time_t start_time, time_t time_interval, void *args,
					sched_cleanup_func_t cleanup_func);

/**
 * @Description: Removes a given task from to the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 *				uuid - uid value of the task to be deleted.
 * @Return: 0 on success, non-zero otherwise.
 * @Complexity: Depends on the complexity of the cleanup function.
**/
int SchedRemoveTask(sched_t *sched, uuid_t uuid);

/**
 * @Description: Activates the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 * @Return: void.
 * @Complexity: varies with the complexity of the operation function and the
 *				the number of times it is called.S 
**/
void SchedRun(sched_t *sched);

/**
 * @Description: Deactivates the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 * @Return: 0 on success, non-zero otherwise.
 * @Complexity: O(1).
**/
int SchedStop(sched_t *sched);

/**
 * @Description: Check if the scheduler is empty.
 * @Parameters: sched - a pointer to the scheduler.
 * @Return: 1 if the scheduler is empty, 0 otherwise.
 * @Complexity: O(1).
**/
int SchedIsEmpty(const sched_t *sched);

/**
 * @Description: Returns the number of tasks currently in the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 * @Return: The number of tasks currently in the scheduler.
 * @Complexity: O(n) (n = number of elements in the scheduler).
**/
size_t SchedSize(const sched_t *sched);

/**
 * @Description: Emptys all the elements from the scheduler.
 * @Parameters: sched - a pointer to the scheduler.
 * @Return: void.
 * @Complexity: Depends on the complexity of the cleanup function.
**/
void SchedClear(sched_t *sched);

#endif /* __SCHEDULER_H_OL108_ILRD__ */

/****************************************************************************** 
 * Auther: Gal Dagan
 * Date: date: 12.08.2021
 * Reviewer: Aharon Luzon
 * Description: Implementation of uid
 
 * Infinity Labs OL108.
 ******************************************************************************/
#ifndef __UID_H_OL108_ILRD__
#define __UID_H_OL108_ILRD__

#include <time.h> 		/* time(), time_t */
#include <sys/types.h> /* ssize_t  and pid_t */

typedef struct uuid
{
	time_t time;
	size_t id;
	pid_t pid;
}uuid_t;

extern const uuid_t bad_uuid;

/**
* @Description: Create a uuid struct
* @Parameters: None.
* @Return: uuid struct with unique values if successful, otherwise returns
* 			bad uuid.
* @Complexity: O(1).
**/
uuid_t UuidCreate(void);

/**
* @Description: Checks if two uuid structs are identical.
* @Parameters: uuid1 - first struct for comparison. uuid2 - second struct for
*				comparison.
* @Return: 1 if equal, 0 otherwise.
* @Complexity: O(1).
**/
int IsSameUuid(uuid_t uuid1, uuid_t uuid2);

#endif /* __UID_H_OL108_ILRD__ */

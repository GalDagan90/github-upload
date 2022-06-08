/***************************************************************************** 
 * Auther: Gal Dagan
 * Date: date: 12.08.2021
 * Reviewer: Aharon Luzon
 * Description: Implementation of uid
 
 * Infinity Labs OL108.
 *****************************************************************************/

#include <unistd.h>	/* getpid() */

#include "uid.h"

const uuid_t bad_uuid = {((time_t)-1), 0, 0};

uuid_t UuidCreate(void)
{
	static size_t id = 1;
	
	uuid_t uuid = {0,0,0};

	uuid.time = time(0);
	if (uuid.time == ((time_t)-1))
	{
		return bad_uuid;
	}
	
	uuid.id = id; 
	uuid.pid = getpid();
	++id;
	
	return uuid;
}

int IsSameUuid(uuid_t uuid1, uuid_t uuid2)
{
	return (uuid1.id == uuid2.id) 
			&& (uuid1.pid == uuid2.pid) 
			&& (uuid1.time == uuid2.time);
}

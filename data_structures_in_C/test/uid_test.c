#include <assert.h> /* assert() */
#include <unistd.h>	/* getpid() */

#include "uid.h"

static void UuidTest(void);

int main(void)
{
	UuidTest();
	
	return 0;
}

static void UuidTest(void)
{
	uuid_t uid1 = UuidCreate();
	uuid_t uid2;
	
	assert(1 == uid1.id);
	assert(getpid() == uid1.pid);
	assert(0 != uid1.time);
	
	sleep(1);
	uid2 = UuidCreate();
	assert(2 == uid2.id);
	assert(getpid() == uid2.pid);
	assert(0 != uid2.time);
	
	assert(uid2.time != uid1.time); 
	
	assert(1 == IsSameUuid(uid1, uid1));
	assert(0 == IsSameUuid(uid2, uid1));
	
}

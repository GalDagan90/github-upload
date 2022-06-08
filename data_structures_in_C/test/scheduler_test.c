#include <stdio.h>
#include <assert.h>
#include "scheduler.h"

#define UNUSED(var) ((void)(var))

int SchedOperation(void *);
void SchedCleanup(void *);
int Stop(void *sched);

int main()
{
	uuid_t uid;
	sched_t *sched = SchedCreate();
	
	int x = 5;
	int y = 43;
	
	SchedAddTask(sched, SchedOperation, time(NULL), 2, &x, SchedCleanup);
	uid = SchedAddTask(sched, SchedOperation, (time(NULL) + 2), 6, &y, SchedCleanup);
	SchedAddTask(sched, SchedOperation, time(NULL) + 2, 3, &y, SchedCleanup);
	assert(!SchedIsEmpty(sched));
	assert(SchedSize(sched) == 3);
	SchedRemoveTask(sched, uid);
	assert(SchedSize(sched) == 2);
	/*SchedClear(sched);
	assert(SchedSize(sched) == 0);*/
	SchedAddTask(sched, Stop, (time(NULL) + 8), 10, sched, SchedCleanup);
	
	SchedRun(sched);
	
	SchedDestroy(sched);
	
	return 0;
}

int SchedOperation(void *data)
{
	*(int *)data += 1;
	
	printf("%d \n", *(int *)data);
	
	if (*(int *)data == 6 || *(int *)data == 44 
		|| *(int *)data == 7 || *(int *)data == 45)
	{
		return 0;
	}
	
	return 1;
}


void SchedCleanup(void *data)
{
	UNUSED(data);
}

int Stop(void *sched)
{
	return SchedStop((sched_t *)sched);
}


/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include <stdio.h>	/* printf() */
#include <signal.h>	/* kill() */

#include "watchdog_shared.h"

#define UNUSED(x) ((void)(x))

int SendSigOperation(void *arg)
{
    UNUSED(arg);

	return 0;
}

int RecieveSigOperation(void *arg)
{
	UNUSED(arg);

	return 0;
}

void InitSched(sched_t *sched, watchdog_data_t *wd)
{
	SchedAddTask(sched, SendSigOperation, time(NULL), wd->signal_to_wd_interval, 
                NULL, SchedCleanUpFunc);
    SchedAddTask(sched, RecieveSigOperation, time(NULL), wd->signal_from_wd_interval,
                wd, SchedCleanUpFunc);
}

void SchedCleanUpFunc(void *arg)
{
	UNUSED(arg);
}



/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __WATCHDOG_SHARED_H_OL108_ILRD__
#define __WATCHDOG_SHARED_H_OL108_ILRD__

#include <unistd.h>	/* pid_t */
#include "watchdog.h"
#include "scheduler.h"

int SendSigOperation(void *arg);

int RecieveSigOperation(void *arg);

void InitSched(sched_t *sched, watchdog_data_t *wd);

void SchedCleanUpFunc(void *arg);


#endif /* __WATCHDOG_SHARED_H_OL108_ILRD__ */

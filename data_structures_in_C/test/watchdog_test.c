/****************************************************************************
*Author: Gal Dagan
*Date: 
*Reviewer: 
*
*Description:
*
*Lab : ol108
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "watchdog.h"

#define UNUSED(x) (void)(x)

int main(int argc, char *argv[], char *envp[])
{
    watchdog_data_t wd_struct = {0};
	pthread_t wd_thread;
	int i = 0;

    UNUSED(argc);
    UNUSED(argv);
    UNUSED(envp);

    wd_struct.process_path = "/home/user/git/ds/watchdog";
    wd_struct.watchdog_path = "/home/user/git/ds/wd_process";
    wd_struct.signal_to_wd_interval = 5;
    wd_struct.signal_from_wd_interval = 5;
    wd_struct.signal_to_wd_miss_limit = 3;
    wd_struct.signal_from_wd_miss_limit = 3;

    wd_thread = StartWatchDog(&wd_struct);

    for (; i < 5; ++i)
	{
		sleep(5);
	}

	EndWatchDog(&wd_thread);
	printf("Test Done\n");

    return (0);
}
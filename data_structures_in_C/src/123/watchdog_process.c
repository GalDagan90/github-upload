#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include <stdio.h>		/* printf()	*/
#include <string.h>     /* atoi(), atol() */
#include <stdlib.h>     /* strtod() */
#include <assert.h>     /* assert() */
#include <sys/types.h>  /* pid_t    */
#include <unistd.h>     /* fork()   */
#include <signal.h>     /* sigaction(), sigemptyset(), sigaddset(),
                        sigprocmask(), sigsuspend(), kill(), struct sigaction, 
                        sigset_t */
#include <pthread.h>    /* pthread_create(), pthread_join() */
#include <time.h>       /* time_t, time() */
#include <semaphore.h>	/* sem_t, sem_open(), sem_wait(), sem_post(), 
						sem_close()	*/
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */

#include "watchdog.h"
#include "scheduler.h"

#define UNUSED(x) ((void)(x))

/******************************************************************************
*                    Global variables                                         *
*******************************************************************************/
static sig_atomic_t misses = 0;
static sigset_t mask;
static int stop_flag = 0;

/*******************************************************************************
*                    Static Function Declaration                               *
*******************************************************************************/
static void GetEnvp(watchdog_data_t *watchdog);
static void SetSignalHandler();
static void WDHandShake(); 
static int SendSigOperation(void *arg);
static int RecieveSigOperation(void *arg);
static void InitSched(sched_t *sched, watchdog_data_t *wd);
static void StopSigHandler(int signum);
void SchedCleanUp(void *arg);

/******************************************************************************/

int main(int argc, char *argv[], char *envp[])
{
    sched_t *sched = NULL;
    watchdog_data_t watchdog = {0};

    printf("In wd_process\n");

	SetSignalHandler(); 

    GetEnvp(&watchdog);
    sched = SchedCreate();
	if (NULL == sched)
	{
		return (1);
	}

	WDHandShake();
    InitSched(sched, &watchdog);
    SchedRun(sched);
	SchedDestroy(sched);
	printf("WD Process done\n");

    UNUSED(argc);
    UNUSED(argv);
    UNUSED(envp);

    return (0);
}

/*****************************************************************/
static void GetEnvp(watchdog_data_t *watchdog)
{
    watchdog->watchdog_path = getenv("watchdog_path");
    watchdog->process_path = getenv("process_path");
    
    watchdog->signal_to_wd_interval = atol(getenv("signal_to_wd_interval"));
    watchdog->signal_from_wd_interval = atol(getenv("signal_from_wd_interval"));
    watchdog->signal_to_wd_miss_limit = atoi(getenv("signal_to_wd_miss_limit"));
    watchdog->signal_from_wd_miss_limit = atoi(getenv("signal_from_wd_miss_limit"));
}

static void SetSignalHandler()
{
    struct sigaction sa;

	sa.sa_flags = 0;
	sa.sa_handler = StopSigHandler;
	sigaction(SIGUSR2, &sa, NULL);

	sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

static void WDHandShake()
{
    sem_t *sem1 = sem_open("/watchdog1", O_CREAT, 0644, 0);

	printf("WD shake hand\n");
    sem_post(sem1);
}

static int SendSigOperation(void *arg)
{	
	if (1 == stop_flag)
	{
		return stop_flag;
	}

	kill(getppid(), SIGUSR1);
    printf("WD Sent\n");
    UNUSED(arg);

    return stop_flag;
}

static int RecieveSigOperation(void *arg)
{
    watchdog_data_t *wd = (watchdog_data_t *)arg;
    struct timespec timepsec = {0,0};

	if (1 == stop_flag)
	{
		return stop_flag;
	}

    if (-1 == sigtimedwait(&mask, NULL, &timepsec))
    {
        printf("WD missed\n");
		++misses;
    }
    else
    {
		printf("WD Received\n");
        misses = 0;
    }

    if (misses >= wd->signal_to_wd_miss_limit)
    {
        execvp(wd->process_path, wd->argv);
    }

    return (0);
}

static void InitSched(sched_t *sched, watchdog_data_t *wd)
{
    SchedAddTask(sched, SendSigOperation, time(NULL), wd->signal_from_wd_interval, 
                NULL, SchedCleanUp);
    SchedAddTask(sched, RecieveSigOperation, time(NULL), wd->signal_to_wd_interval,
                wd, SchedCleanUp);           
}

static void StopSigHandler(int signum)
{
    UNUSED(signum);
	stop_flag = 1;
}

void SchedCleanUp(void *arg)
{
	UNUSED(arg);
}
#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE

#include <stdio.h>      /* sprintf(), atoi(), atol() */
#include <stdlib.h>     /* setenv() */
#include <assert.h>     /* assert() */
#include <sys/types.h>  /* pid_t    */
#include <unistd.h>     /* fork()   */
#include <signal.h>     /* sigemptyset(), sigaddset(),
                        kill(), sigset_t */
#include <pthread.h>    /* pthread_create(), pthread_join(), pthread_sigmask(),
						pthread_t */
#include <time.h>       /* time_t, time() */
#include <semaphore.h>	/* sem_t, sem_open(), sem_wait(), sem_post(), 
						sem_close()	*/
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */						

#include "watchdog.h"
#include "scheduler.h"

#define UNUSED(x) ((void)(x))
#define BUFFER_SIZE 20

/****************************************************************
*                    Global variables                           *
*****************************************************************/
pid_t child_pid;
sig_atomic_t misses = 0;
sigset_t mask;
pthread_mutex_t sig_mutex;
int stop_flag = 0; 

/****************************************************************
*                    Static Function Declaration                *
*****************************************************************/
static void SetEnvp(watchdog_data_t *wd);
static void *ThreadRoutine(void *arg);
static void SetSignalHandler();
static void UPHandShake();
static int SendSigOperation(void *arg);
static int RecieveSigOperation(void *arg);
static void InitSched(sched_t *sched, watchdog_data_t *wd);
void SchedCleanUp(void *arg);

/****************************************************************/

pthread_t StartWatchDog(const watchdog_data_t *wd_data)
{
    pid_t pid;
    pthread_t user_thread;
    
    assert(NULL != wd_data);

    pid = fork();
    if (pid == -1)
    {
        return 0;
    }
    else if (0 == pid)
    {
        SetEnvp((watchdog_data_t *)wd_data);
        execvp(wd_data->watchdog_path, wd_data->argv);
    }
    else
    {
        SetSignalHandler();
        child_pid = pid;
        pthread_create(&user_thread, NULL, ThreadRoutine,
					(watchdog_data_t *)wd_data);
    }

    return user_thread;
}

watchdog_status_t EndWatchDog(pthread_t *watchdog_thread_id)
{
	printf("Ending UP\n");
	stop_flag = 1;
	kill(child_pid, SIGUSR2);
	
	pthread_cancel(*watchdog_thread_id);
	pthread_join(*watchdog_thread_id, NULL);

	return SUCCESS;
}

/******************************************************************************
*                    Static Function Declaration                    		  *
******************************************************************************/
static void SetEnvp(watchdog_data_t *wd)
{
    char buffer[BUFFER_SIZE] = {0};
    
    setenv("watchdog_path", wd->watchdog_path, 1);
    setenv("process_path", wd->process_path, 1);
    
    sprintf(buffer, "%ld", wd->signal_to_wd_interval);
    setenv("signal_to_wd_interval", buffer, 1);
    
    sprintf(buffer, "%ld", wd->signal_from_wd_interval);
    setenv("signal_from_wd_interval", buffer, 1);

    sprintf(buffer, "%d", wd->signal_to_wd_miss_limit);
    setenv("signal_to_wd_miss_limit", buffer, 1);
    
    sprintf(buffer, "%d", wd->signal_from_wd_miss_limit);
    setenv("signal_from_wd_miss_limit", buffer, 1);
}


static void *ThreadRoutine(void *arg)
{
	watchdog_data_t *wd = (watchdog_data_t *)arg;
	sched_t *sched = NULL;

    sched = SchedCreate();
	if (NULL == sched)
	{
		return NULL;
	}
	
	UPHandShake();
    InitSched(sched, wd);
	SchedRun(sched);
	SchedDestroy(sched);

    return NULL;
}

static void UPHandShake()
{
    sem_t *sem1 = sem_open("/watchdog1", O_CREAT, 0644, 0);

	printf("UP shake hand\n");
    
	sem_wait(sem1);
	sem_unlink("watchdog1");
	sem_close(sem1);
}

static void SetSignalHandler()
{
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);
}

static int SendSigOperation(void *arg)
{
    UNUSED(arg);

    kill(child_pid, SIGUSR1);
    printf("UP sent\n");

    return stop_flag;
}

static int RecieveSigOperation(void *arg)
{
    watchdog_data_t *wd = (watchdog_data_t *)arg;
    pid_t pid;
    struct timespec timepsec = {0,0};

    if (1 == stop_flag)
	{
		return (1);
	}

	if (0 > sigtimedwait(&mask, NULL, &timepsec))
    {
        printf("UP missed\n");
        ++misses;
    }
    else
    {
        printf("UP Received\n");
        misses = 0;
    }

    if (misses >= wd->signal_from_wd_miss_limit)
    {
        misses = 0;
        pid = fork();

        if (pid < 0)
        {
            return (1);
        }

        if (0 == pid)
        {
            SetEnvp(wd);
            execvp(wd->watchdog_path, wd->argv);
        }

        child_pid = pid;
    }

    return (0);
}

static void InitSched(sched_t *sched, watchdog_data_t *wd)
{
    SchedAddTask(sched, SendSigOperation, time(NULL), wd->signal_to_wd_interval, 
                NULL, SchedCleanUp);
    SchedAddTask(sched, RecieveSigOperation, time(NULL), wd->signal_from_wd_interval,
                wd, SchedCleanUp);           
}

void SchedCleanUp(void *arg)
{
	UNUSED(arg);
}
#ifndef __WATCHDOG_H_OL107_8_ILRD__
#define __WATCHDOG_H_OL107_8_ILRD__

#include <pthread.h> /* pthread_t */
#include <time.h> /* time_t */

typedef enum
{
	SUCCESS,
	WATCHDOG_CLOSE_FAIL,
	THREAD_CLOSE_FAIL,
	BOTH_CLOSE_FAIL
} watchdog_status_t;

typedef struct watchdog_data
{
	const char *watchdog_path;
	const char *process_path;
	char **argv;
	char **envp;
	time_t signal_to_wd_interval;
	time_t signal_from_wd_interval;
	int signal_to_wd_miss_limit;
	int signal_from_wd_miss_limit;
} watchdog_data_t;


pthread_t StartWatchDog(const watchdog_data_t *wd_data);

watchdog_status_t EndWatchDog(pthread_t *watchdog_thread_id);

#endif /* __WATCHDOG_H_OL107_8_ILRD__ */
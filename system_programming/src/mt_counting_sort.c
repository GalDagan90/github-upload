/****************************************************************************
*Author : Gal Dagan
*Date : 30.10.21	
*Reviewer :	Ido Sapan
*
*Description : Multi-threading counting sort.
*
*Lab : RD5678
****************************************************************************/

#include <string.h>		/* memcpy() */
#include <stdlib.h>		/* malloc(), calloc(), free() */
#include <assert.h> 	/* assert() */
#include <pthread.h>	/* pthread_t, mutex_t, pthread_create(), 
						pthread_join(), pthread_mutex_init(), 
						pthread_mutex_destroy(), pthread_mutex_lock(),
						pthread_mutex_unlock() */

#include "mt_counting_sort.h"

#define UNUSED(x) ((void)(x))
#define CHAR_ARR_SIZE 256

typedef enum status
{
	SUCCESS,
	ALLOC_FAIL,
	THRD_CREATE_FAIL
}status_t;

typedef struct thread_args
{
	pthread_t thread;
	size_t chunk_size;
	char *start_index;
}thread_args_t;

/******************************************************************************/
/*                    Static Function Declaration							  */
/******************************************************************************/ 
void *ThreadRoutine(void *arg);
static int CreateCountingSortThreads(thread_args_t *args, char *arr, 
						size_t size, size_t thread_num );
static int JoinThreads(thread_args_t *args, size_t thread_num);
static void UpdateGlobalHist(int *histogram);
static void AccumulateHist(void);
static int SortArrUsingHist(char *arr, size_t size);
static void InitAttributes(thread_args_t *thread_args, char *arr, 
			size_t chunk_size, size_t runner, size_t thread_num, size_t size);

/******************************************************************************/
int g_histogram[CHAR_ARR_SIZE] = {0};
pthread_mutex_t mutex;

/******************************************************************************/
int MTCountingSort(char *arr, size_t size, size_t thread_num)
{
	thread_args_t *args = NULL;
	status_t status = SUCCESS;

	assert(NULL != arr);
	assert(0 < thread_num);

	memset(g_histogram, 0, sizeof(g_histogram));
	pthread_mutex_init(&mutex, NULL);

	args = (thread_args_t *)malloc(sizeof(thread_args_t) * thread_num);
	if (NULL == args)
	{
		return ALLOC_FAIL;
	}

	status = CreateCountingSortThreads(args, arr, size, thread_num);
	if (SUCCESS != status)
	{
		return status;	
	}

	JoinThreads(args, thread_num);

	AccumulateHist();
	SortArrUsingHist(arr, size);
	pthread_mutex_destroy(&mutex);
	
	free(args);

	return (SUCCESS);
}


/******************************************************************************
*                    Static Function 										  *
*******************************************************************************/
void *ThreadRoutine(void *arg)
{
	thread_args_t *arguments = (thread_args_t *)arg;
	char *runner = arguments->start_index;
	size_t limit = arguments->chunk_size;
	size_t i = 0;
	int local_hist[CHAR_ARR_SIZE] = {0};

	for (; i < limit; ++i, ++runner)
	{
		++local_hist[(unsigned char)(*runner)];
	}

	pthread_mutex_lock(&mutex);
	UpdateGlobalHist(local_hist);
	pthread_mutex_unlock(&mutex);
	
	return arg;
}

static int CreateCountingSortThreads(thread_args_t *args , char *arr, 
									size_t size, size_t thread_num)
{
	size_t runner = 0;

	for (; runner < thread_num; ++runner)
	{
		size_t chunk_size = size / thread_num;
		
		InitAttributes(&args[runner], arr, chunk_size, runner, thread_num, size);

		while (0 != pthread_create(&args[runner].thread , NULL, ThreadRoutine,
								&args[runner]))
		{
			/* empty loop */
		}
	}

	return SUCCESS;
}

static int JoinThreads(thread_args_t *args, size_t thread_num)
{
	size_t runner = 0;
	
	for (runner = 0; runner < thread_num; ++runner)
	{
		pthread_join(args[runner].thread, NULL);
	}

	return SUCCESS;
}

static void UpdateGlobalHist(int *histogram)
{
	size_t runner = 0;

	for (; runner < CHAR_ARR_SIZE; ++runner)
	{
		g_histogram[runner] += histogram[runner];
	}
}

static void AccumulateHist(void)
{
	size_t runner = 1;
	
	for (; runner < CHAR_ARR_SIZE; ++runner)
	{
		g_histogram[runner] += g_histogram[runner - 1];
	}
}

static int SortArrUsingHist(char *arr, size_t size)
{
	char *runner = arr + size - 1;
	char *arr_cpy = (char *)malloc(size + 1);

	if (NULL == arr_cpy)
	{
		return ALLOC_FAIL;
	}

	for (; runner >= arr; --runner)
	{
		arr_cpy[g_histogram[(unsigned char)(*runner)]] = *runner;
		--g_histogram[(unsigned char)(*runner)];
	}

	memcpy(arr, arr_cpy, size);
	free(arr_cpy);

	return SUCCESS;
}

static void InitAttributes(thread_args_t *thread_args, char *arr, 
			size_t chunk_size, size_t runner, size_t thread_num, size_t size)
{
	thread_args->chunk_size = chunk_size;
	thread_args->start_index = arr + chunk_size * runner;

	if (runner == thread_num - 1)
	{
		thread_args->chunk_size = size - chunk_size * runner;
	}
}

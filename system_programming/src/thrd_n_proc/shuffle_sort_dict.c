#define _GNU_SOURCE
#define _POSIX_C_SOURCE (199309L)
#include <stdio.h>		/* printf()	*/
#include <stdlib.h>		/* malloc(), realloc(), free(), rand(), qsort()	*/
#include <assert.h>		/* assert()	*/
#include <string.h>		/* strcpy(), strlen()	*/
#include <stddef.h>		/* size_t	*/
#include <sys/types.h>	/* open()	*/
#include <sys/stat.h>	/* struct stat	*/
#include <fcntl.h>		/* O_ CONSTANTS	*/
#include <sys/mman.h>	/* mmap()	*/
#include <unistd.h>		/* close()	*/
#include <pthread.h>	/* pthread_t, pthread_create(), pthread_join()	*/

#define NS_PER_SECOND (1000000000)
#define NUM_THREADS	10
#define NUM_TESTS 3

typedef struct timespec timespec_t;

typedef enum
{
	SUCCESS = 0,
	FAIL
} status_t;

typedef enum
{
	FALSE = 0,
	TRUE
} bool_t;


typedef struct thread_args
{
	pthread_t thread;
	char **arr;
	size_t thread_index;
	size_t chunk_size;
}thread_args_t;

static size_t g_num_of_ths = 0;

/*****************************************************************************/
static char **CreateDictCopies(size_t, size_t *);
static char **MapDictionary(size_t *);
static char *MapFileToBuffer(const char *, size_t *);
static int CreateThreads(thread_args_t *);
static int ShuffleAndSort(size_t, size_t);
static int RandCmpFunc(const void *, const void *);
static int RealCmpFunc(const void *, const void *);
static int MergeSubs(char **, char **, size_t, size_t);
static void *SortData(void *);
static void ShuffelWords(char **, size_t);
static void CheckSortData(char **, size_t);
static void CleanMemory(char **, thread_args_t *, pthread_t *);
static void SubTimeSpec(timespec_t, timespec_t, timespec_t *);
static void PrintTime(timespec_t, timespec_t, timespec_t, size_t, size_t);
static void InitThreadAttributes(pthread_t *, thread_args_t *, char **, size_t, size_t);	
static size_t CountLines(const char *);
static thread_args_t *CreateThreadsData(pthread_t **, size_t );
/*****************************************************************************/
						   
int main(void)
{
	status_t status = SUCCESS;
	size_t test = 0;
	size_t thread = 0;
	size_t num_of_threads[] = {1, 2, 4 ,8, 10, 16, 20, 32, 64, 128};
	size_t num_of_dicts[] = {1, 5, 10};
	
	for (; test < NUM_TESTS && SUCCESS == status; ++test)
	{
		printf("\nTesting for %lu dictionaries:\n", num_of_dicts[test]);
		for (thread = 0; thread < NUM_THREADS && SUCCESS == status; ++thread)
		{
			status = ShuffleAndSort(num_of_threads[thread], num_of_dicts[test]);
		}
	}
	
	return (SUCCESS);
}

static int ShuffleAndSort(size_t num_of_ths, size_t num_of_dicts)
{
	size_t total_words = 0;
  	timespec_t start, finish, difference;
	thread_args_t *th_data = NULL;	
	pthread_t *threads = NULL;
	char **dict = NULL;

	srand(time(NULL));

	dict = CreateDictCopies(num_of_dicts, &total_words);
	th_data = CreateThreadsData(&threads, num_of_ths);
	if (NULL == th_data || NULL == dict)
	{
		CleanMemory(dict, th_data, threads);

		return (FAIL);
	}

	InitThreadAttributes(threads, th_data, dict, total_words, num_of_ths);

	ShuffelWords(dict, total_words);
	
	g_num_of_ths = num_of_ths;

	clock_gettime(CLOCK_REALTIME, &start);
	
	SortData(th_data);
	
	clock_gettime(CLOCK_REALTIME, &finish);
	
	PrintTime(start, finish, difference, num_of_dicts, num_of_ths);

	CheckSortData(dict, total_words);

	CleanMemory(dict, th_data, threads);

	return (SUCCESS);
}	

static char **CreateDictCopies(size_t num_of_dicts, size_t *total_words)
{
	size_t runner = 0;
	size_t dict_size = 0;
	size_t num_of_words = 0;
	char **combined_arr = NULL;
	char **dict = NULL;

	dict = MapDictionary(&num_of_words);
	if (NULL == dict)
	{
		return (NULL);
	}	

	dict_size = sizeof(char *) * num_of_words;
	*total_words = num_of_words * num_of_dicts;

	combined_arr = (char **)malloc(dict_size * num_of_dicts);
	if (NULL == combined_arr)
	{
		free(dict);
		
		return (NULL);
	}

	for (runner = 0; runner < num_of_dicts; ++runner)
	{
		memcpy((void *)((size_t)combined_arr + (runner * dict_size)), 
			dict, dict_size);
	}

	free(dict);

	return (combined_arr);
}


static char **MapDictionary(size_t *num_of_words)
{
    size_t i = 0;
    size_t runner = 0;
    size_t file_size = 0;
    size_t word_index = 0;
    const char *file_name = "/usr/share/dict/words";
    char **dict = NULL;
	char *buffer = NULL;
    
	*num_of_words = CountLines(file_name);
	
	dict = (char **)malloc(sizeof(char *) * (*num_of_words + 1));
    if (NULL == dict)
    {
    	return (NULL);
    }

    buffer = MapFileToBuffer(file_name, &file_size);
    if (NULL == buffer)
    {
    	free (dict);

    	return (NULL);
    }

	for (i = 0; i < file_size; ++i)
    {
        if ('\n' == buffer[i])
        {
            buffer[i] = '\0';
            dict[runner] = &buffer[word_index];
            word_index = i + 1;
            ++runner;
        }
    }

	buffer[i] = '\0';
    dict[runner] = &buffer[word_index];
    
    return (dict);
}

static size_t CountLines(const char *file_name)
{
	FILE *fp = NULL;
	char ch = 0;
	size_t num_of_words = 0;

    fp = fopen(file_name, "r");
	if (NULL == fp)
	{
		return FAIL;
	}

	while (EOF != (ch = fgetc(fp)))
    {
     	if (ch == '\n')
    	{
    		++num_of_words;
    	}
    }

	fclose(fp);

    return num_of_words;
}

static char *MapFileToBuffer(const char *file_name, size_t *file_size)
{
	struct stat s = {0};
	int fd = 0;

 	fd = open(file_name, O_RDONLY);
	if (-1 == fd)
	{
		return NULL;
	}

    fstat(fd, &s);
	*file_size = s.st_size;

 	return ((char *)mmap(0, *file_size + 1, PROT_READ | PROT_WRITE, 
 						 MAP_PRIVATE, fd, 0));
}

static thread_args_t *CreateThreadsData(pthread_t **threads, size_t num_of_ths)
{
	thread_args_t *th_data = 
			(thread_args_t *)malloc(sizeof(thread_args_t) * num_of_ths);
	
	if (NULL == th_data)
	{
		return (NULL);
	}
	
	memset(th_data, 0, sizeof(thread_args_t) * num_of_ths);

	*threads = (pthread_t *)malloc(sizeof(pthread_t) * num_of_ths);
	if (NULL == *threads)
	{	
		free(th_data);

		return (NULL);
	}
	
	memset(*threads, 0, sizeof(pthread_t) * num_of_ths);

	return (th_data);
}	

static void InitThreadAttributes(pthread_t *threads, thread_args_t *th_data, 
						   char **data, size_t words_in_dict, size_t num_of_ths)
{	
	size_t runner = 0;
	size_t chunk_size = 0;

	chunk_size = words_in_dict / num_of_ths;

	for (runner = 0; runner < num_of_ths; ++runner)
	{
		th_data[runner].thread = threads[runner];
		th_data[runner].arr = data + (runner * chunk_size);
		th_data[runner].chunk_size = chunk_size;
		th_data[runner].thread_index = runner;
	}

	th_data[runner - 1].chunk_size = 
						words_in_dict - (chunk_size * (num_of_ths - 1));
}

static void ShuffelWords(char **dict, size_t total_words)
{
	qsort(dict, total_words, sizeof(char *), RandCmpFunc);
}

static int RandCmpFunc(const void *data1, const void *data2)
{
	(void)data1;
	(void)data2;
	
	return (rand() - RAND_MAX / 2);
}	

static int RealCmpFunc(const void *data1, const void *data2)
{
	return (strcmp(*(char **)data1, *(char **)data2));
}

static void CheckSortData(char **data, size_t data_size)
{
	size_t i = 0;

	for (i = 1; i < data_size; ++i)
	{
		if (0 < strcmp(*(data + i - 1), *(data + i)))
		{
			printf("FAIL: %s comes before %s\n",
				   *(data + i - 1), *(data + i));
		}
	}
}	

static void CleanMemory(char **data, thread_args_t *th_data, pthread_t *threads)
{
	g_num_of_ths = 0;
	
	free(data);
	free(th_data);
	free(threads);
}

static void PrintTime(timespec_t start, timespec_t finish, timespec_t diff, 
					  size_t num_of_dicts, size_t num_of_ths)
{
	SubTimeSpec(start, finish, &diff);
	printf("%ld threads - %ld dicts - qsort: ", num_of_ths, num_of_dicts);
	printf("%d.%.ld sec\n", (int)diff.tv_sec, diff.tv_nsec);
}

static void SubTimeSpec(timespec_t t1, timespec_t t2, timespec_t *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}	

static void *SortData(void *arg)
{
	thread_args_t *th_data = (thread_args_t *)arg;
	
	qsort(th_data->arr, th_data->chunk_size, sizeof(char *),
			RealCmpFunc);
	
	if (0 == th_data->thread_index)
	{
		if (SUCCESS != CreateThreads(th_data))
		{
			return (NULL);
		}
	}

	return (NULL);
}

static int CreateThreads(thread_args_t *th_data_arr)
{
	size_t i = 1;

	for (i = 1; i < g_num_of_ths; ++i)
	{
		if (SUCCESS != pthread_create(&th_data_arr[i].thread, NULL,
									     SortData, &th_data_arr[i]))
		{
			return (FAIL);
		}									 
	}
	
	for (i = 1; i < g_num_of_ths; ++i)
	{
		if (SUCCESS != pthread_join(th_data_arr[i].thread, NULL))
		{
			return (FAIL);
		}
		
		MergeSubs(th_data_arr[0].arr, th_data_arr[i].arr,
				  th_data_arr[0].chunk_size, th_data_arr[i].chunk_size);
				  
		th_data_arr[0].chunk_size += th_data_arr[1].chunk_size; 										 
	}

	return (SUCCESS);
}

static int MergeSubs(char **left, char **right, size_t left_num_of_words, 
												size_t right_num_of_words)
{
	char **left_runner = NULL;
	char **right_runner = NULL;
	char **merged = NULL;
	char **merged_runner = NULL;
	size_t merged_num_of_words = 0;
	size_t counter = 0;

	merged_num_of_words = left_num_of_words + right_num_of_words;

	left_runner = left;
	right_runner = right;
	merged = (char **)malloc((merged_num_of_words) * sizeof(char *));
	if (NULL == merged)
	{
		return (FAIL);
	}

	merged_runner = merged;
	
	while (counter < merged_num_of_words)
	{
		if ((size_t)(left_runner - left) >= left_num_of_words)
		{
			*merged_runner = *right_runner;
			++merged_runner;
			++right_runner;
			++counter;
		}
		else if ((size_t)(right_runner - right) >= right_num_of_words)
		{
			*merged_runner = *left_runner;
			++merged_runner;
			++left_runner;
			++counter;			
		}
		else if (strcmp(*left_runner, *right_runner) < 0)
		{
			*merged_runner = *left_runner;
			++merged_runner;
			++left_runner;
			++counter;
		}
		else
		{
			*merged_runner = *right_runner;
			++merged_runner;
			++right_runner;			
			++counter;
		}
	}

	memcpy(left, merged, merged_num_of_words * sizeof(char *));
	
	free(merged);

	return (SUCCESS);
}

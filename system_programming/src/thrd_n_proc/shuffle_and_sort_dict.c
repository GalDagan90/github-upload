/****************************************************************************
*Author : Gal Dagan
*Date : 30.10.21	
*Reviewer :	Ido Sapan
*
*Description : Multi-threading counting sort.
*
*Lab : RD5678
****************************************************************************/
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

#define WORD_BUFFER 50
#define UNUSED(x) (void)(x)

typedef enum status
{
	SUCCESS,
	FAIL
}status_t;

typedef struct thread_args
{
	pthread_t thread;
	size_t chunk_size;
	size_t start_index;
	char **arr;
}thread_args_t;


static char *ReadFileIntoMemory(const char *file_dir, size_t *file_size);
static char **CreateCombinedArr(char *buffer, size_t file_size);
static void ShuffleArr(char **combined_arr, size_t size);
static void SortArr(char **combined_arr, size_t size, size_t thread_num);
static void MergeSortedArr(char **combined_arr, size_t size, size_t thread_num);
static void MergeTwoSortedArrays(char **arr1, size_t size_1, char **arr2, 
							size_t size_2, char **merged_arr);
static void CopyToCombinedArr(char **combined_arr, char **merged_arr, 
							size_t size);
static void PlaceInMergedArr(char **merged_arr, size_t merge_index, char **arr, 
							size_t arr_index);
static void *ThreadRoutine(void *arg);
static size_t CountLines(char *buffer);
static int RandCmpFunc(const void *data1, const void *data2);
static int RealCmpFunc(const void *data1, const void *data2);
static void InitAttributes(thread_args_t *args, char **arr, 
			size_t chunk_size, size_t runner, size_t thread_num, size_t size);
static void FreeArr(char **combined_arr, size_t size);


int main(void)
{
	char *file_dir = "/usr/share/dict/american-english";
	char *buffer = NULL;
	char **combined_arr = NULL;
	size_t size = 0;
	size_t file_size = 0;
	size_t runner = 0;

	buffer = ReadFileIntoMemory(file_dir, &file_size);
	combined_arr = CreateCombinedArr(buffer, file_size);
	size = CountLines(buffer);

	/* ShuffleArr(combined_arr, size); */
	
	/* for (; runner < size; ++runner)
	{
		printf("%s ", combined_arr[runner]);
	} */
	/*SortArr(combined_arr, size, 4);
	MergeSortedArr(combined_arr, size, 4);

	FreeArr(combined_arr, size); */

	return (0);
}

static char *ReadFileIntoMemory(const char *file_dir, size_t *file_size)
{
	char *buffer = NULL;
	int fd = 0;
	struct stat stat_struct;

	assert(NULL != file_dir);

	fd = open(file_dir, O_RDONLY);
	if (-1 == fd)
	{
		return NULL;
	}

	fstat(fd, &stat_struct);
	*file_size = stat_struct.st_size;
	
	buffer = (char *)mmap(0, *file_size, PROT_READ, MAP_PRIVATE, fd, 0);

	close(fd);

	return buffer;
}

static char **CreateCombinedArr(char *buffer, size_t file_size)
{
	char **dict_words = NULL;
	size_t num_lines = CountLines(buffer);
	size_t runner = 0;
	size_t word_index = 0;
	size_t i = 0;

	dict_words = (char **)malloc(sizeof(char *) * num_lines);
	if (NULL == dict_words)
	{
		return NULL;
	}

	for (; i < file_size; ++i)
	{
		if ('\n' == buffer[i])
		{
			buffer[i] = '\0';
			dict_words[runner] = &buffer[word_index];
			word_index = i + 1;
			++runner;
		}
	}

	/* for (; '\0' != *buffer && runner < num_lines; ++runner, ++buffer)
	{
		char word[WORD_BUFFER] = {'\0'};
		int i = 0;

		for (; '\0' != *buffer && '\n' != *buffer && i < WORD_BUFFER; 
			++buffer, ++i)
		{
			word[i] = *buffer;
		}
		
		word[i] = '\0';
		dict_words[runner] = (char *)malloc(sizeof(char) * (i + 1));
		if (NULL == dict_words[runner])
		{
			return NULL;
		}
		strcpy(dict_words[runner], word);
	} */

	return dict_words;
}

static void ShuffleArr(char **combined_arr, size_t size)
{
	qsort(combined_arr, size, sizeof(char *), &RandCmpFunc);
}

static void SortArr(char **combined_arr, size_t size, size_t thread_num)
{
	size_t runner = 0;
	size_t chunk_size = size / thread_num;
	thread_args_t *args = NULL;

	args = (thread_args_t *)malloc(sizeof(thread_args_t) * thread_num);
	if (NULL == args)
	{
		return;
	}

	for (; runner < thread_num; ++runner)
	{
		InitAttributes(&args[runner], combined_arr, chunk_size, runner, 
					thread_num, size);

		while (SUCCESS != pthread_create(&args[runner].thread, NULL, ThreadRoutine,
								&args[runner]))
		{ 
			/* empty loop */
		}
	}

	for (runner = 0; runner < thread_num; ++runner)
	{
		pthread_join(args[runner].thread, NULL);
	}

	free(args);
}

static void *ThreadRoutine(void *arg)
{
	thread_args_t *argument = (thread_args_t *)arg;
	size_t nmemb = argument->chunk_size;
	size_t start_index = argument->start_index;

	qsort(&argument->arr[start_index], nmemb, sizeof(char *), RealCmpFunc);

	return NULL;
}

static void MergeSortedArr(char **combined_arr, size_t size, size_t thread_num)
{
	size_t runner = 0;
	size_t chunk_size = size / thread_num;
	thread_args_t *args = 
				(thread_args_t *)malloc(sizeof(thread_args_t) * thread_num);
	char **merged_arr = (char **)malloc(sizeof(char *) * size);

	if (NULL == merged_arr || NULL == args)
	{
		return;
	}

	for (; runner < thread_num; ++runner)
	{
		InitAttributes(&args[runner], combined_arr, chunk_size, runner, 
					thread_num, size);
	}

	for (runner = 1; runner < thread_num; ++runner)
	{
		MergeTwoSortedArrays(combined_arr + args[0].start_index, args[0].chunk_size, 
			combined_arr + args[runner].start_index, args[runner].chunk_size, merged_arr);
		
		args[0].chunk_size += args[runner].chunk_size;
		
		CopyToCombinedArr(combined_arr, merged_arr, args[0].chunk_size);
	}

	free(merged_arr);
	free(args);
}

static void MergeTwoSortedArrays(char **arr1, size_t size_1, char **arr2, 
							size_t size_2, char **merged_arr)
{
    size_t runner_1 = 0;
    size_t runner_2 = 0;
    size_t merge_runner = 0;

    for (; (runner_1 < size_1) && (runner_2 < size_2); ++merge_runner)
    {
		if (strcmp(arr1[runner_1], arr2[runner_2]) < 0)
        {
			PlaceInMergedArr(merged_arr, merge_runner, arr1, runner_1);
			++runner_1;
        }
        else
        {
			PlaceInMergedArr(merged_arr, merge_runner, arr2, runner_2);
			++runner_2;
        }
    }

    for (; runner_1 < size_1; ++runner_1, ++merge_runner)
    {
        PlaceInMergedArr(merged_arr, merge_runner, arr1, runner_1);
    }
    for (; runner_2 < size_2; ++runner_2, ++merge_runner)
    {
        PlaceInMergedArr(merged_arr, merge_runner, arr2, runner_2);
    }
}

static void CopyToCombinedArr(char **combined_arr, char **merged_arr, 
							size_t size)
{
	size_t i= 0;

	for (i = 0; i < size; ++i)
	{
		combined_arr[i] = realloc(combined_arr[i], strlen(merged_arr[i]) + 1);
		strcpy(combined_arr[i], merged_arr[i]);
		free(merged_arr[i]);
	}	
}

static void PlaceInMergedArr(char **merged_arr, size_t merge_index, char **arr, 
							size_t arr_index)
{
	merged_arr[merge_index] = (char *)malloc(strlen(arr[arr_index]) + 1);
	strcpy(merged_arr[merge_index], arr[arr_index]);
}

static size_t CountLines(char *buffer)
{
	char delim = '\n';
	size_t new_line_counter = 0;

	while('\0' != *buffer)
	{
		if (delim == *buffer)
		{
			++new_line_counter;
		}

		++buffer;
	}

	return new_line_counter;
}

static int RandCmpFunc(const void *data1, const void *data2)
{
	UNUSED(data1);
	UNUSED(data2);

	return (rand() - RAND_MAX / 2);
}

static int RealCmpFunc(const void *data1, const void *data2)
{
	return (strcmp(*(char **)data1, *(char **)data2));
}

static void InitAttributes(thread_args_t *args, char **arr, 
			size_t chunk_size, size_t runner, size_t thread_num, size_t size)
{
	args->chunk_size = chunk_size;
	args->start_index = chunk_size * runner;
	args->arr = arr;

	if (runner == thread_num - 1)
	{
		args->chunk_size = size - chunk_size * runner;
	}
}

static void FreeArr(char **combined_arr, size_t size)
{
	size_t runner = 0;

	for (; runner < size; ++runner)
	{
		free(combined_arr[runner]);
	}
	free(combined_arr);
}
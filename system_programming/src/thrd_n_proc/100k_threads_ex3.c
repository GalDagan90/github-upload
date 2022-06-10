#include <stdio.h>      /* printf() */ 
#include <assert.h>     /* assert() */
#include <stdlib.h>     /* exit()   */
#include <pthread.h>    /* pthread_create() */
#include <time.h>       /* time()   */
#include <unistd.h>     /* sleep()  */

#define NUM_THRD 100000

int arr[NUM_THRD] = {0};
pthread_t thread_arr[NUM_THRD];

void *ThreadRoutine(void *index);

int main(void)
{
    int thread_runner = 0;
    size_t valid_index = 0;
    size_t failed_threads = 0;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    for (; thread_runner < NUM_THRD; ++thread_runner)
    {
        if (0 != pthread_create(&thread_arr[thread_runner], &attr, 
                                &ThreadRoutine, &thread_runner))
        {
            ++failed_threads;
        }
    }

    sleep(3);
    
    for (thread_runner = 0; thread_runner < NUM_THRD ; ++thread_runner)
    {
        if (arr[thread_runner] == thread_runner)
        {
            ++valid_index;
        }
    } 
    
    pthread_attr_destroy(&attr);
    printf("Failed: %lu\n", failed_threads);
    printf("Valid: %lu\n", valid_index);
    printf("Total: %lu\n", failed_threads + valid_index);

    pthread_exit(0);
}

void *ThreadRoutine(void *index)
{   
    arr[(*(size_t *)(index))] = (*(size_t *)(index));

    return NULL;
}
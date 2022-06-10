#include <stdio.h>      /* printf() */ 
#include <assert.h>     /* assert() */
#include <stdlib.h>     /* exit()   */
#include <pthread.h>    /* pthread_create(), pthread_join() */
#include <unistd.h>     /* sleep()  */
#include <sys/time.h>
#include <sys/resource.h>

#define NUM_THRD 8
#define BIG_NUM 100000000

void *ThreadRoutine(void *arg);

pthread_t thread_arr[NUM_THRD];

int main(void)
{
    size_t thread_runner = 0;
    size_t res = 0;
    
    for (; thread_runner < NUM_THRD; ++thread_runner)
    {
        size_t *initial_div = (size_t *)malloc(sizeof(size_t));
        *initial_div = ((size_t)(BIG_NUM / NUM_THRD) + 1) * thread_runner;
        
        if (0 != pthread_create(&thread_arr[thread_runner], NULL, 
                                &ThreadRoutine, initial_div))
        {
            perror("Failed to create thread\n");
        }
    }
    
    for (thread_runner = 0; thread_runner < NUM_THRD; ++thread_runner)
    {
        size_t *temp_res;
        pthread_join(thread_arr[thread_runner], (void **)&temp_res);
        res += *temp_res;
        free(temp_res);
    }

    printf("%lu\n", res);

    return (0);
}

void *ThreadRoutine(void *arg)
{   
    size_t divisor = *(size_t *)arg;
    size_t i = 1;
    size_t sum_of_divisors = 0;

    for (; ((i + divisor) < BIG_NUM) && i <= ((size_t)(BIG_NUM / NUM_THRD) + 1); ++i)
    {
        if (0 == BIG_NUM % (i + divisor))
        {
            sum_of_divisors += (i + divisor);
        }
    }

    *(size_t *)arg = sum_of_divisors;
    
    return arg;
}
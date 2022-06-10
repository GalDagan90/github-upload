/****************************************************************************
*Author : Gal Dagan
*Date : 18.10.21
*Reviewer : Liel Sananes
*
*Description : Quick & Dirty implementation of producer & consumer problem
*
*Lab : ol108
****************************************************************************/
#include <stdio.h>	    /* printf() */
#include <stdlib.h>
#include <pthread.h>    /* pthread_t, pthread_create(), pthread_join(), 
                        pthread_mutex_t, pthread_mutex_init(), pthread_mutex_destroy()
                        pthread_mutex_lock(), pthread_mutex_unlock(), 
                        pthread_cond_init(), pthread_cond_destroy(), 
                        pthread_cond_broadcast() */

#define NUM_OF_THREADS 3
#define LOOP 10

int g_counter = 0;
static int g_static_counter = 0;

/***************************************************************************
*                           Static Function Decleration                    *
****************************************************************************/
static void *RecThreadRoutine(void *arg);

/***************************************************************************/

int main(void)
{
    pthread_t thread_arr[NUM_OF_THREADS];
    int runner = 0;


    for (; runner < NUM_OF_THREADS; ++runner)
    {
        pthread_create(&thread_arr[runner], NULL, RecThreadRoutine, NULL);
    }
    
    for (runner = 0; runner < NUM_OF_THREADS; ++runner)
    {
        pthread_join(thread_arr[runner], NULL);
    }

    return (0);
}

static void *RecThreadRoutine(void *arg)
{
    arg = malloc(sizeof(short));

    for (; g_counter < LOOP; ++g_counter)
    {
        *(short *)arg = g_static_counter;
        ++(*(short *)arg);
        g_static_counter += 2;
    }

    return RecThreadRoutine(arg);
}
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
#include <pthread.h>    /* pthread_t, pthread_create(), pthread_join(), 
                        pthread_mutex_t, pthread_mutex_init(), pthread_mutex_destroy()
                        pthread_mutex_lock(), pthread_mutex_unlock(), 
                        pthread_cond_init(), pthread_cond_destroy(), 
                        pthread_cond_broadcast() */
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <semaphore.h>  /* sem_t, sem_init(), sem_destroy(), sem_wait(), 
                        sem_post() */

#include "producer_consumer.h"
#include "doubly_linked_list.h"
#include "queue.h" 

#define FSQ_SIZE 4
#define NUM_OF_MSG 5 
#define NUM_OF_CONS 2

/***************************************************************************
*                           Static Function Decleration                    *
***************************************************************************/ 
static void *Producer_1(void *arg);
static void *Consumer_1(void *arg);

static void *Producer_2(void *arg);
static void *Consumer_2(void *arg);

static void *Producer_3(void *arg);
static void *Consumer_3(void *arg);

static void *Producer_4(void *arg);
static void *Consumer_4(void *arg); 

static void *Producer_5(void *arg);
static void *Consumer_5(void *arg); 

static void *Producer_6(void *arg);
static void *Consumer_6(void *arg);

static void *Producer_8(void *arg);
static void *Consumer_8(void *arg);

/**************************************************************************/
/*                          Global variables                              */
/**************************************************************************/ 
int g_counter = 0;

pthread_mutex_t ex_2_mutex;

sem_t ex_3_sem;

sem_t ex_4_sem_empty;
sem_t ex_4_sem_full;
pthread_mutex_t ex_4_mutex;

sem_t ex_5_sem_prod;
sem_t ex_5_sem_cons;
pthread_mutex_t ex_5_mutex_prod;
pthread_mutex_t ex_5_mutex_cons;

__sig_atomic_t atomic_counter_1 = 0;
__sig_atomic_t atomic_counter_2 = 0;
pthread_mutex_t ex_6_mutex;

pthread_mutex_t ex_8_mutex;
sem_t ex_8_sem;
pthread_cond_t ex_8_cond;
int msg_array[5] = {1,2,3,4,5};
int msg_buffer[1] = {0};
int g_consumer = 0;

/**************************************************************************/

int Exercise8(void)
{
    pthread_t cons_thread_arr[NUM_OF_CONS];
    pthread_t prod_thread;
    int thrd_runner = 0;

    pthread_mutex_init(&ex_8_mutex, NULL);
    pthread_cond_init(&ex_8_cond, NULL);
    sem_init(&ex_8_sem, 0, 0);

    pthread_create(&prod_thread, NULL, Producer_8, NULL);
    
    for (; thrd_runner < NUM_OF_CONS; ++thrd_runner)
    {
        pthread_create(&cons_thread_arr[thrd_runner], NULL, Consumer_8, NULL);
    }    
    
    pthread_join(prod_thread, NULL);

    for (thrd_runner = 0; thrd_runner < NUM_OF_CONS; ++thrd_runner)
    {
        pthread_join(cons_thread_arr[thrd_runner], NULL);
    }
    
    pthread_mutex_destroy(&ex_8_mutex);
    pthread_cond_destroy(&ex_8_cond);
    sem_destroy(&ex_8_sem);

    return (0);
}

static void *Producer_8(void *arg)
{
    int runner = 0;
    g_counter = 0;

    for (; runner < NUM_OF_MSG; ++runner)
    {
        int i = 0;
        g_consumer = NUM_OF_CONS;

        pthread_mutex_lock(&ex_8_mutex);
        printf("massege: %d\n", msg_array[runner]);
        msg_buffer[0] = msg_array[runner];

        for (; i < NUM_OF_CONS; ++i)
        {
            sem_post(&ex_8_sem);
        }
        pthread_cond_wait(&ex_8_cond, &ex_8_mutex);
        
        pthread_mutex_unlock(&ex_8_mutex);
    }

    return arg;
}

static void *Consumer_8(void *arg)
{
    int runner = 0;

    for (; runner < NUM_OF_MSG; ++runner)
    {  
        sem_wait(&ex_8_sem);
        pthread_mutex_lock(&ex_8_mutex);

        printf("consumed: %d\n", msg_buffer[0]);
        --g_consumer;

        if (0 == g_consumer)
        {
            pthread_cond_broadcast(&ex_8_cond);
        }
        else
        {
            pthread_cond_wait(&ex_8_cond, &ex_8_mutex);
        }

        pthread_mutex_unlock(&ex_8_mutex);
    }
    
    return arg;
}


int Exercise6(void)
{
    int num_of_threads = 20;
    int thrd_runner = 0;
    pthread_t thread_arr[20];
    queue_t *queue = QueueCreate();

    pthread_mutex_init(&ex_6_mutex, NULL);

    for (; thrd_runner < num_of_threads; ++thrd_runner)
    {
        if (0 == (thrd_runner % 2))
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Producer_6, queue);
        }
        else
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Consumer_6, queue);
        }
    }
    
    for (thrd_runner = 0; thrd_runner < num_of_threads; ++thrd_runner)
    {
        pthread_join(thread_arr[thrd_runner], NULL);
    }
    
    pthread_mutex_destroy(&ex_6_mutex);
    QueueDestroy(queue);

    return (0);
}
 
static void *Producer_6(void *arg)
{   
    queue_t *queue = (queue_t *)arg;
    
    pthread_mutex_lock(&ex_6_mutex);
    if (atomic_counter_1 < FSQ_SIZE)
    {    
        printf("Enqueuing\n");
        QueueEnqueue(queue, &g_counter);
        ++atomic_counter_1;
        ++atomic_counter_2;
    }

    pthread_mutex_unlock(&ex_6_mutex);

    return arg;
}

static void *Consumer_6(void *arg)
{
    queue_t *queue = (queue_t *)arg;
    
    pthread_mutex_lock(&ex_6_mutex);
    if (atomic_counter_2 > 0)
    {
        printf("Dequeuing\n");
        QueueDequeue(queue);
        --atomic_counter_1;
        --atomic_counter_2;
    }
    
    pthread_mutex_unlock(&ex_6_mutex);
    
    return arg;
}


int Exercise5(void)
{
    int num_of_threads = 20;
    int thrd_runner = 0;
    pthread_t thread_arr[20];
    queue_t *queue = QueueCreate();

    pthread_mutex_init(&ex_5_mutex_prod, NULL);
    pthread_mutex_init(&ex_5_mutex_cons, NULL);
    sem_init(&ex_5_sem_prod, 0, FSQ_SIZE);
    sem_init(&ex_5_sem_cons, 0, 0);

    for (; thrd_runner < num_of_threads; ++thrd_runner)
    {
        if (0 == (thrd_runner % 2))
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Producer_5, queue);
        }
        else
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Consumer_5, queue);
        }
    }
    
    for (thrd_runner = 0; thrd_runner < num_of_threads; ++thrd_runner)
    {
        pthread_join(thread_arr[thrd_runner], NULL);
    }
    
    pthread_mutex_destroy(&ex_5_mutex_prod);
    pthread_mutex_destroy(&ex_5_mutex_cons);
    sem_destroy(&ex_5_sem_prod);
    sem_destroy(&ex_5_sem_cons);
    QueueDestroy(queue);

    return (0);
}
 
static void *Producer_5(void *arg)
{   
    queue_t *queue = (queue_t *)arg;
    
    sem_wait(&ex_5_sem_prod);
    pthread_mutex_lock(&ex_5_mutex_prod);
    
    printf("Enqueuing\n");
    QueueEnqueue(queue, &g_counter);

    pthread_mutex_unlock(&ex_5_mutex_prod);
    sem_post(&ex_5_sem_cons);

    return arg;
}

static void *Consumer_5(void *arg)
{
    queue_t *queue = (queue_t *)arg;
    
    sem_wait(&ex_5_sem_cons);
    pthread_mutex_lock(&ex_5_mutex_cons);
    
    printf("Dequeuing\n");
    QueueDequeue(queue);

    pthread_mutex_unlock(&ex_5_mutex_cons);
    sem_post(&ex_5_sem_prod);
    
    return arg;
}


int Exercise4(void)
{
    int num_of_threads = 20;
    int thrd_runner = 0;
    pthread_t thread_arr[20];
    queue_t *queue = QueueCreate();

    pthread_mutex_init(&ex_4_mutex, NULL);
    sem_init(&ex_4_sem_empty, 0, FSQ_SIZE);
    sem_init(&ex_4_sem_full, 0, 0);

    for (; thrd_runner < num_of_threads; ++thrd_runner)
    {
        if (0 == (thrd_runner % 2))
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Producer_4, queue);
        }
        else
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Consumer_4, queue);
        }
    }
    
    for (thrd_runner = 0; thrd_runner < num_of_threads; ++thrd_runner)
    {
        pthread_join(thread_arr[thrd_runner], NULL);
    }
    
    pthread_mutex_destroy(&ex_4_mutex);
    sem_destroy(&ex_4_sem_empty);
    sem_destroy(&ex_4_sem_full);
    QueueDestroy(queue);

    return (0);
}
 
static void *Producer_4(void *arg)
{   
    queue_t *queue = (queue_t *)arg;

    sem_wait(&ex_4_sem_empty);
    pthread_mutex_lock(&ex_4_mutex);
    
    printf("Enqueuing\n");
    QueueEnqueue(queue, &g_counter);

    pthread_mutex_unlock(&ex_4_mutex);
    sem_post(&ex_4_sem_full);

    return arg;
}

static void *Consumer_4(void *arg)
{
    queue_t *queue = (queue_t *)arg;
    
    sem_wait(&ex_4_sem_full);
    pthread_mutex_lock(&ex_4_mutex);
    
    printf("Dequeuing\n");
    QueueDequeue(queue);

    pthread_mutex_unlock(&ex_4_mutex);
    sem_post(&ex_4_sem_empty);
    
    return arg;
}


int Exercise3(void)
{
    int num_of_threads = 20;
    int thrd_runner = 0;
    pthread_t thread_arr[20];
    dlist_t *dll = DListCreate();

    g_counter = 0;
    pthread_mutex_init(&ex_2_mutex, NULL);
    sem_init(&ex_3_sem, 0, num_of_threads / 4);

    for (; thrd_runner < num_of_threads; ++thrd_runner)
    {
        if (0 == (thrd_runner % 2))
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Producer_3, dll);
        }
        else
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Consumer_3, dll);
        }
    }
    
    for (thrd_runner = 0; thrd_runner < num_of_threads; ++thrd_runner)
    {
        pthread_join(thread_arr[thrd_runner], NULL);
    }
    
    pthread_mutex_destroy(&ex_2_mutex);
    sem_destroy(&ex_3_sem);
    DListDestroy(dll);

    return (0);
}

static void *Producer_3(void *arg)
{   
    dlist_t *dll = (dlist_t *)arg;

    sem_wait(&ex_3_sem);
    pthread_mutex_lock(&ex_2_mutex);
    
    printf("Pushing\n");
    DListPushFront(dll, &g_counter);

    pthread_mutex_unlock(&ex_2_mutex);
    sem_post(&ex_3_sem);

    return arg;
}

static void *Consumer_3(void *arg)
{
    dlist_t *dll = (dlist_t *)arg;
    
    sem_post(&ex_3_sem);
    pthread_mutex_lock(&ex_2_mutex);
    
    while (!DListIsEmpty(dll))
    {
        printf("popping\n") ;
        DListPopFront(dll);
    }

    pthread_mutex_unlock(&ex_2_mutex);
    sem_wait(&ex_3_sem);
    
    return arg;
}


int Exercise2(void)
{
    int num_of_threads = 10;
    int thrd_runner = 0;
    pthread_t thread_arr[10];
    dlist_t *dll = DListCreate();

    g_counter = 0;
    pthread_mutex_init(&ex_2_mutex, NULL);

    for (; thrd_runner < num_of_threads; ++thrd_runner)
    {
        if (0 == (thrd_runner % 2))
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Producer_2, dll);
        }
        else
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Consumer_2, dll);
        }
    }
    
    for (thrd_runner = 0; thrd_runner < num_of_threads; ++thrd_runner)
    {
        pthread_join(thread_arr[thrd_runner], NULL);
    }
    
    pthread_mutex_destroy(&ex_2_mutex);
    DListDestroy(dll);

    return (0);
}

static void *Producer_2(void *arg)
{   
    dlist_t *dll = (dlist_t *)arg;

    pthread_mutex_lock(&ex_2_mutex);
    printf("Pushing %d\n", g_counter);
    DListPushFront(dll, &g_counter);
    ++g_counter;
    pthread_mutex_unlock(&ex_2_mutex);
    
    return arg;
}

static void *Consumer_2(void *arg)
{
    dlist_t *dll = (dlist_t *)arg;
    
    pthread_mutex_lock(&ex_2_mutex);
    
    while (!DListIsEmpty(dll))
    {
        printf("List Size: %lu\n", DListGetSize(dll));
        printf("popping\n");
        DListPopFront(dll);
        --g_counter;   
    }

    pthread_mutex_unlock(&ex_2_mutex);
    
    return arg;
}


int Exercise1(void)
{
    int num_of_threads = 2;
    int thrd_runner = 0;
    pthread_t thread_arr[2];

    for (; thrd_runner < num_of_threads; ++thrd_runner)
    {
        if (0 == (thrd_runner % 2))
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Producer_1, NULL);
        }
        else
        {
            pthread_create(&thread_arr[thrd_runner], NULL, Consumer_1, NULL);
        }
    }
    
    for (thrd_runner = 0; thrd_runner < num_of_threads; ++thrd_runner)
    {
        pthread_join(thread_arr[thrd_runner], NULL);
    }

    return (0);
}

static void *Producer_1(void *arg)
{   
    int i = 0;
    
    while (i < 5)
    {
        if (0 == g_counter)
        {
            printf("sent\n");
            ++g_counter;
            ++i;
        }
    }

    return arg;
}

static void *Consumer_1(void *arg)
{
    int i = 0;

    while (i < 5)
    {
        if (1 == g_counter)
        {
            printf("received\n");
            --g_counter;
            ++i;
        }
    }
    
    return arg;
}

/****************************************************************************
*Author : Gal Dagan
*Date : 17.10.21
*Reviewer : Keren Robbins
*
*Description : Named semaphore implementation
*
*Lab : ol108
****************************************************************************/
#include <string.h> /* strcpy() */
#include <stdlib.h> /* strtod() */
#include <stdio.h>  /* fgets()  */
#include <fcntl.h>      /* For O_* constants */
#include <sys/stat.h>   /* For mode constants */
#include <semaphore.h>  /* sem_open(), sem_unlink(), sem_wait(),
                        sem_post() */

#include "sem_manipulation.h"

#define UNUSED(x) (void)(x)
#define SEM_NAME_LEN 250
#define BUFFER_SIZE 20
#define DELIMITER " "
#define END_OF_STRING "\0"
#define NEW_LINE "\n"

typedef enum valid_keys
{
    I,
    D,
    V,
    NUM_VALID_KEYS
}keys_t;

typedef enum undo_caller
{
    INC,
    DEC
}undo_caller_t; 

typedef enum input_type
{
    NUMBER,
    UNDO,
    NUM_OF_ARGS
}input_type_t; 

typedef struct oper
{
    int (*oper_func_t)(sem_t *, char *, int *, int *);
}oper_t;

/***************************************************************************
*                   Static Functions Declaration                           *
****************************************************************************/
char *GetSemName(char *sem_name, const char *input_str);
void GetCommands(sem_t *semaphore, int *undo_counter);
int IncrementSem(sem_t *sem, char *input_str, 
                    int *undo_counter, int *parsed_params);
int DecrementSem(sem_t *sem, char *input_str, 
                    int *undo_counter, int *parsed_params);
int GetSemValue(sem_t *sem, char *input_str, 
                    int *undo_counter, int *parsed_params);
int PerformExitCommand(sem_t *sem, int *undo_counter);
void HandleInput(char *input_str, int *parsed_params);
void HandleUndo(int *undo_counter, undo_caller_t undo_type, 
                int *parsed_params);

/***************************************************************************
*                               LUT                                        *
****************************************************************************/
oper_t g_key_options[NUM_VALID_KEYS] = {
    {IncrementSem},
    {DecrementSem},
    {GetSemValue}
}; 

/***************************************************************************/

int SemManipulation(int argc, char *argv[])
{   
    char sem_name[SEM_NAME_LEN] = {'\0'};
    int undo_counter = 0;
    sem_t *semaphore;

    if (argc < 2)
    {
        return 1;
    }

    GetSemName(sem_name, argv[1]);
    printf("sem: %s\n", sem_name);

    semaphore = sem_open(sem_name, O_CREAT, 0644, 0);
    
    GetCommands(semaphore, &undo_counter);
    
    PerformExitCommand(semaphore, &undo_counter);
    sem_close(semaphore);
    sem_unlink(sem_name);

    return (0);
}

/***************************************************************************
*                   Static Functions Implementation                        *
****************************************************************************/
char *GetSemName(char *sem_name, const char *input_str)
{
    *sem_name = '/';
    strcpy(sem_name + 1, input_str);

    return sem_name;
}

void GetCommands(sem_t *semaphore, int *undo_counter)
{
    char input_buffer[BUFFER_SIZE] = {'\0'};
    int parsed_params[NUM_OF_ARGS] = {0};

    fgets(input_buffer, BUFFER_SIZE, stdin);

    while ('X' != input_buffer[0])
    {
        switch (input_buffer[0])
        {
            case 'I':
                g_key_options[I].oper_func_t(semaphore, input_buffer, 
                                            undo_counter, parsed_params);
                break;
            case 'D':
                g_key_options[D].oper_func_t(semaphore, input_buffer, 
                                            undo_counter, parsed_params);
                break;
            case 'V':
                g_key_options[V].oper_func_t(semaphore, input_buffer, 
                                            undo_counter, parsed_params);
                break;
            default:
                break;
        }
        
        fgets(input_buffer, BUFFER_SIZE, stdin);
    }
}

void HandleInput(char *input_str, int *parsed_params)
{
    char *runner = NULL;
    int num = 0;

    runner = strtok(input_str, DELIMITER);
    runner = strtok(NULL, END_OF_STRING);

    if (NULL != runner)
    {
        num = strtod(runner, &input_str);
    }     

    parsed_params[NUMBER] = num;

    runner = strtok(input_str, NEW_LINE);
    if (NULL == runner)
    {
        parsed_params[UNDO] = 0;
    }
    else if (0 == strcmp(runner, " UNDO"))
    {
        parsed_params[UNDO] = 1;
    }
    else
    {
        parsed_params[UNDO] = -1;
        parsed_params[NUMBER] = 0;
    }
    
}

void HandleUndo(int *undo_counter, undo_caller_t undo_type, 
                int *parsed_params)
{
    if (1 == parsed_params[UNDO] && INC == undo_type)
    {
        *undo_counter -= parsed_params[NUMBER];
    }
    else if (1 == parsed_params[UNDO] && DEC == undo_type)
    {
        *undo_counter += parsed_params[NUMBER];
    }
}

int IncrementSem(sem_t *sem, char *input_str, 
                    int *undo_counter, int *parsed_params)
{   
    HandleInput(input_str, parsed_params);
    HandleUndo(undo_counter, INC, parsed_params);

    for (; parsed_params[NUMBER] > 0; --parsed_params[NUMBER])
    {
        sem_post(sem);
    }
    
    return (0);
}

int DecrementSem(sem_t *sem, char *input_str, 
                    int *undo_counter, int *parsed_params)
{
    HandleInput(input_str, parsed_params);
    HandleUndo(undo_counter, DEC, parsed_params);

    for (; parsed_params[NUMBER] > 0; --parsed_params[NUMBER])
    {
        sem_wait(sem);
    }
    
    return (0);
}

int GetSemValue(sem_t *sem, char *input_str, 
                    int *undo_counter, int *parsed_params)
{
    int sval = -1;

    sem_getvalue(sem, &sval);
    printf("Semaphore value: %d\n", sval);
    
    UNUSED(input_str);
    UNUSED(parsed_params);
    UNUSED(undo_counter);
    return (0);
}

int PerformExitCommand(sem_t *sem, int *undo_counter)
{
    for (;*undo_counter > 0; --(*undo_counter))
    {
        sem_post(sem);
    }
    for (;*undo_counter < 0; ++(*undo_counter))
    {
        sem_wait(sem);
    }

    *undo_counter = 0;

    return (0);
}


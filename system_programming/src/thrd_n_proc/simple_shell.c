/**************************************************************
* Author: Gal Dagan
* Date: 04.10.21
* Reviewer: Eyal Dafni
*
* Description: Simple shell implementation using system(),
*           fork(), and execvp() syscalls
*
* InfintyLabs OL108
**************************************************************/
#include <stdio.h>      /* fgets(), printf(), fprintf() */  
#include <stdlib.h>     /* system() */ 
#include <sys/types.h>  /* pid_t    */
#include <unistd.h>     /* fork(), execvp() */
#include <string.h>     /* strcmp(), strtok(), strlen() */
#include <sys/wait.h>   /* wait() */

#define MAX_ARG_LIST_LEN 1000
#define BUFFER_SIZE 1024

static char *ReadInput(char *buffer);
static void ParseInput(char *input_str, char **arg_list);
static void GetCommandWithSystem(void);
static void GetCommandWithFork(void);
static int IsExitCommand(char *str);

int main(void)
{
    printf("Phase 1:\n");
    GetCommandWithSystem();
    
    printf("\n");
    printf("Phase 2:\n");
    GetCommandWithFork();

    return (0);
} 

static void GetCommandWithSystem(void)
{
    char buffer[BUFFER_SIZE] = {'\0'};

    ReadInput(buffer);
    
    while (!IsExitCommand(buffer))
    {
        system(buffer);
        ReadInput(buffer);
    }
}

static void GetCommandWithFork(void)
{
    char buffer[BUFFER_SIZE] = {'\0'};

    ReadInput(buffer);

    while (!IsExitCommand(buffer))
    {
        pid_t child_pid = fork();  

        if (child_pid != 0) 
        {
            wait(NULL); 
        }
        else 
        {
            char *arg_list[MAX_ARG_LIST_LEN] = {NULL};

            ParseInput(buffer, arg_list);
            execvp(arg_list[0], arg_list);
            
            fprintf(stderr, "Invalid command\n");
            abort();
        }

        ReadInput(buffer);
    }
}

static char *ReadInput(char *buffer)
{
    printf("Enter a command to execute (enter \"q\" to quit):\n");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strlen(buffer) - 1] = '\0';

    return buffer;
}

static void ParseInput(char *input_str, char **arg_list)
{
    char *word_runner = NULL;
    int list_runner = 0;
    
    strtok(input_str, "\0");
    word_runner = strtok(input_str, " ");

    for (; NULL != word_runner; ++list_runner, word_runner = strtok(NULL, " "))
    {
        arg_list[list_runner] = word_runner;
    }

    arg_list[list_runner] = NULL;
}

static int IsExitCommand(char *str)
{
    return (0 == strcmp(str, "q"));
}
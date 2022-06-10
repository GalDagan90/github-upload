/**************************************************************
* Author: Gal Dagan
* Reviewer: Navot Yaari
* Date: 06.10.21
*
* Description: signals ping-pong
*
* InfintyLabs OL108
**************************************************************/
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>      /* printf() */  
#include <stdlib.h>     /* exit()   */
#include <sys/types.h>  /* pid_t    */
#include <unistd.h>     /* fork()   */
#include <signal.h>     /* sigaction(), sigemptyset(), sigaddset(),
                        sigprocmask(), sigsuspend(), kill(), struct sigaction, 
                        sigset_t */

#define UNUSED(x) ((void)(x))

void SigHandler(int signum);

int main() 
{ 

    pid_t pid = 0;
    sigset_t mask;
    sigset_t old_mask;
    struct sigaction sa = {0};

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, &old_mask);
    sa.sa_handler = &SigHandler;

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    pid = fork();
    if (-1 == pid)
    {
        return (-1);
    }

    if (pid != 0)
    {
        size_t counter = 0;

        for (; counter < 5; ++counter)
        {
            printf("Ping\n");
            kill(pid, SIGUSR1);
            sigsuspend(&old_mask);
        }
    }
    else 
    {
        size_t counter = 0;

        for (; counter < 5; ++counter)
        {
            sigsuspend(&old_mask);
            printf("Pong\n");
            kill(getppid(), SIGUSR2);
        }
    }
    
    sa.sa_handler = SIG_DFL;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    return (0);
}

void SigHandler(int signum)
{
    UNUSED(signum);
}
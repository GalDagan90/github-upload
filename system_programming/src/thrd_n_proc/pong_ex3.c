/*********************************************************
 *	Author: Gal Dagan
 *	Reviewer: Navot Yaari
 *	Date: 06.10.2021.
 *
 *	Description: signals ping pong ex3 - child
 *
 *	Infinity Labs OL108
 *********************************************************/
#define _POSIX_C_SOURCE 199309L

#include <stdio.h> /* printf() */

#include <signal.h>     /* sigaction(), kill(), SIGUSR1, SIGUSR2, sigemptyset()
                           sigaddset(), sigprocmask(), sigwaitinfo()
                           sigsuspend(), struct sigaction, sigset_t, siginfo_t */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* getpid() */
#include <string.h>     /* memset() */
#include <stdlib.h>     /* strtod() */

#define UNUSED(x) ((void)(x))
#define NUM 10000

static pid_t pid = 0;
static sig_atomic_t sig_counter = 0;
/************* function declare *******************************/
static void PongHandler(int signum);
static void WaitForSignal(sigset_t *oldmask);
static void SetBlockedSignal(sigset_t *oldmask, int signum);
static void SetSignalHandler(void (*handler_func)(int), int signum);
/************* main ****************************************/
int main(int argc, char *argv[])
{
    sigset_t oldmask;

    if (argc <= 1)
    {
        printf("pass pid as arg\n");

        return (1);
    }

    SetSignalHandler(PongHandler, SIGUSR1);
    SetBlockedSignal(&oldmask, SIGUSR1);

    pid = (pid_t)(strtod(argv[1], NULL));
    kill(pid, SIGUSR2);

    while (NUM > sig_counter)
    {
        WaitForSignal(&oldmask);
        kill(pid, SIGUSR2);
    }

    printf("%d\n", sig_counter);

    return (0);
}

/************************************************************/

static void PongHandler(int signum)
{
    UNUSED(signum);
    ++sig_counter;
}

static void WaitForSignal(sigset_t *oldmask)
{
    sig_atomic_t counter = sig_counter;

    sigfillset(oldmask);
    sigdelset(oldmask, SIGUSR1);

    while (counter == sig_counter)
    {
        sigsuspend(oldmask);
    }
}

static void SetBlockedSignal(sigset_t *oldmask, int signum)
{
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, signum);
    sigprocmask(SIG_BLOCK, &mask, oldmask);
}

static void SetSignalHandler(void (*handler_func)(int), int signum)
{
    struct sigaction sa = {0};

    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = handler_func;
    sigaction(signum, &sa, NULL);
}
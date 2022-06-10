/*********************************************************
 *	Author: Gal Dagan.
 *	Reviewer: Navot Yaari
 *	Date: 06.10.2021.
 *
 *	Description: signals_ping_pong ex2 - child
 *
 *	Infinity Labs OL108
 *********************************************************/
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>      /* printf() */
#include <signal.h>     /* sigaction(), kill(), sigfillset(), sigdelset(), 
                           sigemptyset(), sigaddset(), sigprocmask(), 
                           sigsuspend() */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* getppid(), fork(), execl() */
#include <string.h>     /* memset() */

#define UNUSED(x) ((void)(x))
#define NUM 10000

static sig_atomic_t sig_counter = 0;

/************************************************************
*                function declaration                       *
*************************************************************/                           
static void SigHandlerChild(int signum);
static void SetSignalHandler(void (*handler_func)(int), int signum);
static void SetBlockedSignal(sigset_t *oldmask, int signum);
static void WaitForSignal(sigset_t *oldmask);

/************************************************************/

int main(void)
{
    sigset_t oldmask;

    SetSignalHandler(SigHandlerChild, SIGUSR1);
    SetBlockedSignal(&oldmask, SIGUSR1);

    kill(getppid(), SIGUSR2);
    while (NUM > sig_counter)
    {
        WaitForSignal(&oldmask);
        kill(getppid(), SIGUSR2);
    }

    printf("%d\n", sig_counter);

    return (0);
}

/************* Implementation *********************************/
static void SigHandlerChild(int signum)
{
    UNUSED(signum);
    ++sig_counter;
}

static void SetSignalHandler(void (*handler_func)(int), int signum)
{
    struct sigaction sa = {0};

    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = handler_func;
    sigaction(signum, &sa, NULL);
}

static void SetBlockedSignal(sigset_t *oldmask, int signum)
{
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, signum);
    sigprocmask(SIG_BLOCK, &mask, oldmask);
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
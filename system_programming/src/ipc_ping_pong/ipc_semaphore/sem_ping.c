#include <stdio.h>			/* puts */
#include <fcntl.h>			/* For O_* constants */
#include <sys/stat.h>		/* For mode constants */
#include <semaphore.h>		/* sem_t, sem_open, sem_wait, sem_post */

#define LOOP_NUM 5

int main(int argc, char *argv[])
{
	sem_t *sem_ping;
	sem_t *sem_pong;
	char *ping_name = argv[1];
	char *pong_name = argv[2];
	size_t counter = 0;

	sem_ping = sem_open(ping_name, O_CREAT, 0777, 1);
	sem_pong = sem_open(pong_name, O_CREAT, 0777, 0);

	while (counter < LOOP_NUM)
	{
		sem_wait(sem_ping);

		puts("ping");

		sem_post(sem_pong);
		++counter;
	}

	sem_close(sem_ping);
	sem_close(sem_pong);
	sem_unlink(ping_name);
	sem_unlink(pong_name);

	return (0);
}
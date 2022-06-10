#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>		/* For O_* constants */
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

#define PIPE_ARR 2
#define BUFFER_LEN 5

char g_ping[] = "ping";
char g_pong[] = "pong";

int main()
{
	int ping_fd = 0;
	int pong_fd = 0;
	int i = 0;

	mkfifo("ping_fifo", 0666);
	mkfifo("pong_fifo", 0666);

	if (-1 == (ping_fd = open("ping_fifo", O_WRONLY)))
	{
		return 1;
	}
	if (-1 == (pong_fd = open("pong_fifo", O_RDONLY)))
	{
		return 1;
	}

	for (; i < 5; ++i)
	{
		char buffer[BUFFER_LEN];

		write(ping_fd, g_ping, BUFFER_LEN);

		read(pong_fd, buffer, BUFFER_LEN);
		printf("Recieved: %s\n", buffer);
		sleep(2);
	}
	close(ping_fd);
	close(pong_fd);

	return (0);
}
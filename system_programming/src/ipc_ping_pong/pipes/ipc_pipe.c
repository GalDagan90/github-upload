#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>		/* For O_* constants */
#include <sys/wait.h>

#define PIPE_ARR 2
#define BUFFER_LEN 5

char g_ping[] = "ping";
char g_pong[] = "pong";

int main()
{
	int parent_fds[PIPE_ARR]; 	/* parent -> child */
	int child_fds[PIPE_ARR];	/* child -> parent */
	int parent_read;
	int parent_write;
	int child_read;
	int child_write;
	pid_t pid;

	pipe(parent_fds);
	parent_read = parent_fds[0];
	parent_write = parent_fds[1];
	pipe(child_fds);
	child_read = child_fds[0];
	child_write = child_fds[1];

	pid = fork();
	if (-1 == pid)
	{
		printf("Fork Error\n");
		return 1;
	}
	if (0 == pid) /* close the parent write, open parent read, print, write in child pipe */
	{
		char buffer[BUFFER_LEN];
		int i = 0;

		while (i < 5)
		{
			read(parent_read, buffer, BUFFER_LEN);
			printf("Child recieved from parent: %s\n", buffer);

			write(child_write, g_pong, BUFFER_LEN);
			++i;
		}
		close(parent_read);
		close(child_write);
	}
	else
	{
		char buffer[BUFFER_LEN];
		int i = 0;

		while (i < 5)
		{
			write(parent_write, g_ping, BUFFER_LEN);

			read(child_read, buffer, BUFFER_LEN);
			printf("Parent recieved from child: %s\n", buffer);
			++i;
		}
		close(parent_write);
		close(child_read);
	}

	wait(NULL);
	return (0);
}
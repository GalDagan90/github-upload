#include <sys/types.h>
#include <sys/socket.h>	/* socket(), bind(), setsockopt(), listen() */
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>		/* strcmp()	*/
#include <iostream>		/* cout */
#include <stdio.h>
#include <sys/select.h>	/* select()	*/
#include <sys/time.h> 	/* struct timeval */

#include "udp_tcp.hpp"
#include "server_ping_pong.hpp"

char message[] = "pong";
const char *ping = "ping";
char file_path[] = "/home/user/git/system_programming/src/udp_tcp/logger.txt";
const time_t g_sec = 7;
const std::size_t g_buffer_size = 1024;
FILE *fp = NULL;
char buffer[g_buffer_size] = {0};

/******************************************************************************
*							Static Functions decleration					  *
******************************************************************************/ 

static void InitTimeOutVal(struct timeval *timeout);
static int MaxFd(int fd_1, int fd_2);
static int OpenAndBindUDPSocket(int *udp_fd, struct sockaddr_in *server_addr, 
								const int *on, const int port);
static int OpenAndBindTCPSocket(int *tcp_fd, struct sockaddr_in *server_addr, 
								const int *on, const int port);
static void PrepareSet(int udp_fd, int tcp_fd, fd_set *read_set, fd_set *master_set);
static int HandleConnections(int tcp_fd, int *fd_max, 
		fd_set *read_set, fd_set *master_set, struct sockaddr_in *remote_addr, 
		socklen_t *addrlen, bool *running);
static void TimedOut();
static void HandleSTDIN(bool *running);
static int AcceptNewConnection(int tcp_fd, fd_set *read_set, fd_set *master_set,
					 struct sockaddr_in *remote_addr, socklen_t *addrlen);
static void ReceiveAndSend(int fd, struct sockaddr_in *remote_addr, 
						socklen_t *len, fd_set *master_set);
static void LogAction(const char *str);

/******************************************************************************/

int CreatePingPongServer(int port)
{
	fd_set read_set, master_set;
	struct sockaddr_in tcp_server_addr;
	struct sockaddr_in udp_server_addr;
	int tcp_fd, udp_fd, fd_max;
	const int on = 1;

	fp = fopen(file_path, "w");
	if (NULL == fp)
	{
		return (1);
	}

	if (OpenAndBindUDPSocket(&udp_fd, &udp_server_addr, &on, port) == -1)
	{
		return (1);
	}
	
	if (OpenAndBindTCPSocket(&tcp_fd, &tcp_server_addr, &on, port) == -1)
	{
		return (1);
	}

	PrepareSet(udp_fd, tcp_fd, &read_set, &master_set);

	fd_max = MaxFd(udp_fd, tcp_fd) + 1;

	bool running = true;
	while (running)
	{
		struct timeval timeout;
		struct sockaddr_in remote_addr;
		socklen_t len = sizeof(remote_addr);

		InitTimeOutVal(&timeout);
		read_set = master_set;
		
		int ready_fd = select(fd_max, &read_set, NULL, NULL, &timeout);
		if (-1 == ready_fd)
		{
			perror("Select error\n");
			running = false;
		}
		else if (0 == ready_fd)
		{
			TimedOut();
		}
		else 
		{
			HandleConnections(tcp_fd, &fd_max, &read_set, &master_set, 
							&remote_addr, &len, &running);
		}
	}

	fclose(fp);
	close(tcp_fd);
	close(udp_fd);

	return (0);
}

static int OpenAndBindUDPSocket(int *udp_fd, struct sockaddr_in *server_addr, 
								const int *on, const int port)
{
	*udp_fd = CreateUDPSocket();
	if (-1 == *udp_fd)
	{
		perror("Create UDP socket\n");
		return (-1);
	}

	InitServerStructAddr(server_addr, port, htonl(INADDR_ANY));
	setsockopt(*udp_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT | SO_BROADCAST,
	 			on, sizeof(*on));
	if (BindServer(*udp_fd, server_addr) == -1)
	{
		perror("UDP Bind error\n");
		return (-1);
	}

	return (*udp_fd);
}

static int OpenAndBindTCPSocket(int *tcp_fd, struct sockaddr_in *server_addr, 
								const int *on, const int port)
{
	*tcp_fd = CreateTCPSocket();
	if (-1 == *tcp_fd)
	{
		perror("Create TCP socket\n");
		return (-1);
	}

	InitServerStructAddr(server_addr, port, inet_addr("127.0.0.1"));
	setsockopt(*tcp_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, on, sizeof(*on));
	if (BindServer(*tcp_fd, server_addr) == -1)
	{
		perror("TCP Bind error\n");
		return (-1);
	}

	if (listen(*tcp_fd, 10) == -1)
	{
		perror("listen\n");
		return(-1);
	}

	return (*tcp_fd);
}

static void PrepareSet(int udp_fd, int tcp_fd, fd_set *read_set, fd_set *master_set)
{
	FD_ZERO(read_set);
	FD_ZERO(master_set);

	FD_SET(tcp_fd, master_set);
	FD_SET(udp_fd, master_set);
	FD_SET(STDIN_FILENO, master_set);
}

static void TimedOut()
{
	LogAction("Timed out, trying again");
}

static int HandleConnections(int tcp_fd, int *fd_max, 
		fd_set *read_set, fd_set *master_set, struct sockaddr_in *remote_addr, 
		socklen_t *addrlen, bool *running)
{
	if (FD_ISSET(STDIN_FILENO, read_set))
	{
		HandleSTDIN(running);
	}
	else if (FD_ISSET(tcp_fd, read_set))
	{
		int new_fd = AcceptNewConnection(tcp_fd, read_set, master_set, 
							remote_addr, addrlen);
		if (-1 == new_fd)
		{
			return (-1);
		}

		*fd_max = MaxFd(*fd_max, new_fd) + 1;
	}
	for (int runner_fd = 1; runner_fd < *fd_max; ++runner_fd)
	{
		if (FD_ISSET(runner_fd, read_set))
		{
			ReceiveAndSend(runner_fd, remote_addr, addrlen, master_set);
		}
	}

	return (0);
}

static void HandleSTDIN(bool *running)
{
	std::size_t n_bytes = read(STDIN_FILENO, buffer, g_buffer_size);
	buffer[n_bytes - 1] = '\0';

	if (0 == strcmp(buffer, ping))
	{
		LogAction(message);
	}
	if (0 == strcmp(buffer, "quit"))
	{
		LogAction("quit");
		*running = false;
	}
}

static int AcceptNewConnection(int tcp_fd, fd_set *read_set, fd_set *master_set,
					 struct sockaddr_in *remote_addr, socklen_t *addrlen)
{
	int new_fd = accept(tcp_fd, (struct sockaddr*)remote_addr, addrlen);
	if (-1 == new_fd)
	{
		perror("accept\n");
		return (-1);
	}
	else
	{
		FD_SET(new_fd, master_set);
		FD_ZERO(read_set);
		FD_SET(new_fd, read_set);
	}

	return new_fd;
}
static void ReceiveAndSend(int fd, struct sockaddr_in *remote_addr, 
						socklen_t *len, fd_set *master_set)
{
	int n_bytes = RecvFrom(fd, buffer, g_buffer_size, remote_addr, len);
	if (n_bytes == -1)
	{
		FD_CLR(fd, master_set);
		LogAction("Client Closed");
		close(fd);
		return;
	}

	buffer[n_bytes] = '\0';
	LogAction("client recieved ping");

	SentTo(fd, message, g_buffer_size, remote_addr, len);
	LogAction("Server returned pong");
}

static void LogAction(const char *str)
{
	fputs(str, fp);
	fputs("\n",fp);
	std::cout << str << std::endl;
}

static void InitTimeOutVal(struct timeval *timeout)
{
	timeout->tv_sec =  g_sec;
	timeout->tv_usec = 0;
}

static int MaxFd(int fd_1, int fd_2)
{
	return ((fd_1 > fd_2) ? fd_1 : fd_2);
}

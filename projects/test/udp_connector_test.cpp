/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include <cstdio>
#include <iostream>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

#include "udp_connector.hpp"

#define UNUSED(x) (void)(x)
#define PORT 5000
#define PORT2 4950

using namespace framework;

static void InitTimeOutVal(struct timeval *timeout);

const int buffer_size = 1024;
char server_message[] = "Server\n";
char client_message[] = "client\n";
char addr[] = "127.0.0.1";
char buffer[buffer_size] = {0};

int main()
{
	UDPSocket udp_server(addr, PORT, addr, PORT2);
	fd_set read_set;
	fd_set master_set;
	int fd_max;

	FD_ZERO(&read_set);
	FD_SET(udp_server.GetFD(), &master_set);

	fd_max = udp_server.GetFD() + 1;

	int max_timouts = 10;
	while (max_timouts)
	{
		struct timeval timeout;
		InitTimeOutVal(&timeout);
		read_set = master_set;
		
		int ready_fd = select(fd_max, &read_set, NULL, NULL, &timeout);
		if (-1 == ready_fd)
		{
			perror("Select error\n");
			max_timouts = false;
		}
		else if (0 == ready_fd)
		{
			std::cout << "Timeout\n" << std::endl;
			--max_timouts;
		}
		else 
		{
			for (int runner_fd = 0; runner_fd < fd_max; ++runner_fd)
			{
				if (FD_ISSET(runner_fd, &read_set))
				{
					udp_server.Recv(buffer, buffer_size);
					std::cout << "Server recieved: " << buffer << std::endl;
					udp_server.Send(server_message, buffer_size);
				}
			}
		}
	}

	return 0;
}

static void InitTimeOutVal(struct timeval *timeout)
{
	timeout->tv_sec =  3;
	timeout->tv_usec = 0;
}

#include <iostream>
#include <string>

#include "udp_connector.hpp"

using namespace wordle_game;

const int selfPort = 5000;
const int clientPort = 4950;
const char *IP = "192.168.14.101";

static void InitTimeOutVal(struct timeval *timeout)
{
	timeout->tv_sec =  3;
	timeout->tv_usec = 0;
}

struct server_test
{
	int id;
};

struct sender_test
{
	std::string text;
};


int main()
{
	UDPConnector server(selfPort, IP, clientPort, IP);

	fd_set read_set;
	fd_set master_set;
	int fd_max;

	FD_ZERO(&read_set);
	FD_SET(server.GetFD(), &master_set);

	fd_max = server.GetFD() + 1;

	int max_timouts = 4;
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
					char buffer[100];
					server.Recv(buffer, 100);
					std::string text = buffer;
					std::cout << "Server recieved: " << text << std::endl;

					server_test server_pack;
					server_pack.id = 1;
					server.Send(reinterpret_cast<char *>(&server_pack), sizeof(server_pack));
				}
			}
		}
	}

	return 0;
}
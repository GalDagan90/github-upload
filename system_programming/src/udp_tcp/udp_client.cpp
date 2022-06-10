#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "udp_tcp.hpp"

#define PORT 4950

const std::size_t buffer_size = 256;
char message[] = "ping";

int main()
{
	char buffer[buffer_size];
	struct sockaddr_in server_addr;
	socklen_t len = sizeof(server_addr);
	int on = 1;

	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_BROADCAST;

	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

	for (int i = 0; i < 5; ++i)
	{
		struct sockaddr_in remote_addr;
		sendto(sock_fd, message, buffer_size, 0, (struct sockaddr *)&server_addr, 
				sizeof(server_addr));
		std::cout << "Client sent: " << message << std::endl;

		recvfrom(sock_fd, buffer, buffer_size, 0, (struct sockaddr *)&remote_addr, &len);
		std::cout << "Client recieved: " << buffer << std::endl;

		sleep(3);
	}

	close(sock_fd);

	return (0);
}
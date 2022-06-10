#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "udp_tcp.hpp"

#define PORT 5000

const std::size_t buffer_size = 256;

int main()
{
	char buffer[buffer_size];
	struct sockaddr_in server_addr;
	int broadcast = 1;
	
	int server_id = CreateUDPSocket();
	setsockopt(server_id, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR | SO_REUSEPORT, 
				&broadcast, sizeof(broadcast));
	InitServerStructAddr(&server_addr, PORT, INADDR_ANY);
	BindServer(server_id, &server_addr);

	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);

	while (1)
	{
		RecvFrom(server_id, buffer, buffer_size, &client_addr, &addrlen);

		std::cout << "Server recieved: " << buffer << std::endl;
	}

	close(server_id);

	return (0);
}

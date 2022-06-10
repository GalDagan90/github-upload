#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "udp_tcp.hpp"

#define PORT 5000

const std::size_t buffer_size = 256;
char message[] = "Hello from server";

int main()
{
	char buffer[buffer_size];
	struct sockaddr_in server_addr;
	int opt = 1;
	
	int server_id = CreateUDPSocket();

	setsockopt(server_id, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
				&opt, sizeof(opt));

	InitServerStructAddr(&server_addr, PORT, INADDR_ANY);
	BindServer(server_id, &server_addr);

	for (int i = 0; i < 5; ++i)
	{
		struct sockaddr_in client_addr;
		socklen_t addrlen = sizeof(client_addr);

		RecvFrom(server_id, buffer, buffer_size, &client_addr, &addrlen);
		std::cout << "Server recieved: " << buffer << std::endl;
		SentTo(server_id, message, buffer_size, &client_addr, &addrlen);
	}

	close(server_id);

	return (0);
}

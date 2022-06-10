#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "udp_tcp.hpp"

#define PORT 5000
const std::size_t buffer_size = 256;
char message[] = "For good time cal 1-800-good-time";

int main()
{
	struct sockaddr_in recv_addr;  
	int broadcast = 1;

	int socket_id = CreateUDPSocket();

	setsockopt(socket_id, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

	InitClientStructAddr(&recv_addr, PORT, INADDR_BROADCAST);

	while (1)
	{
		unsigned int addrlen = sizeof(recv_addr);

		SentTo(socket_id, message, buffer_size, &recv_addr, &addrlen);
		sleep(3);
	}

	close(socket_id);

	return (0);
}

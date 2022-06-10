#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#include "udp_tcp.hpp"

const int port = 5000;
const std::size_t buffer_size = 256;
char message[] = "Hello from Server";

int main()
{
	int sock;
	int sock_fd = CreateTCPSocket();
	int broadcast = 1;

	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
				&broadcast, sizeof(broadcast));

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	InitServerStructAddr(&server_addr, port, INADDR_ANY);
	BindServer(sock_fd, &server_addr);
	listen(sock_fd, 3);

	char buffer[buffer_size];
	int len = sizeof(struct sockaddr_in);

	sock = accept(sock_fd, (struct sockaddr *)&client_addr, 
				(socklen_t *)&len);

	for (int i = 0; i < 5; ++i)
	{
		int n_bytes = recv(sock, buffer, buffer_size, 0);
		buffer[n_bytes] = '\0';
		std::cout << "Server recieved: " << buffer << std::endl;

		send(sock, message, strlen(message) + 1, 0);
	}
	close(sock);

	return (0);
}
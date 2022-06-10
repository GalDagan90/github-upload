#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

const int port = 4950;
const std::size_t buffer_size = 256;
const char message[] = "TCP Ping";
char buffer[buffer_size];



int main()
{
	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

	connect(sock_desc, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
	for (int i = 0; i < 5; ++i)
	{
		struct sockaddr_in remote; 
		socklen_t len = sizeof(remote);
		
		sendto(sock_desc, message, strlen(message) + 1, 0, (struct sockaddr *)&server, sizeof(server));
		std::cout << "Sent: " << message << std::endl;

		int n_bytes = recvfrom(sock_desc, buffer, buffer_size, 0, 
				(struct sockaddr *)&remote, &len);
		buffer[n_bytes] = '\0';

		std::cout << "Recieved: " << buffer << std::endl;

		sleep(3);
	}

	close(sock_desc);

	return (0);
}
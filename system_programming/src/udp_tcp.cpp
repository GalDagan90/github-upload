#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "udp_tcp.hpp"

int CreateUDPSocket()
{
	int res = socket(AF_INET, SOCK_DGRAM, 0);

	return res;
}

int CreateTCPSocket()
{
	int res = socket(AF_INET, SOCK_STREAM, 0);

	return res;
}

void InitClientStructAddr(struct sockaddr_in *addr, const int port, int s_addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons((int)port);
	addr->sin_addr.s_addr = s_addr;
}

void InitServerStructAddr(struct sockaddr_in *addr, const int port, int s_addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = htons((int)port);
	addr->sin_addr.s_addr = s_addr;
}

int BindServer(int sock_fd, struct sockaddr_in *addr)
{
	return (bind(sock_fd, (struct sockaddr *)addr, sizeof(struct sockaddr_in)));
}

int SentTo(int sock_fd, void *data, const std::size_t data_size, 
			struct sockaddr_in *recv_addr, unsigned int *addrlen)
{
	return sendto(sock_fd, data, data_size, 0, (struct sockaddr *)recv_addr, 
				*addrlen);
}

int RecvFrom(int sock_fd, void *buffer, const std::size_t buffer_size, 
			struct sockaddr_in *recv_addr, unsigned int *addrlen)
{
	int n_bytes_rec = recvfrom(sock_fd, buffer, buffer_size, 0, 
								(struct sockaddr *)recv_addr, addrlen);

	return n_bytes_rec;
}

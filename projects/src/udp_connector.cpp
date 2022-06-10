/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#include <arpa/inet.h>
#include <iostream>

#include "udp_connector.hpp"

namespace framework
{


/******************************************************************************
*								Class UDP	    							  *
*******************************************************************************/

UDPSocket::UDPSocket(const char *selfIP, size_t selfPort, 
				const char *IPtoConnect, size_t portToConnect) : m_fd(-1)
{
	m_fd = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in self_addr= GetSockaddr(selfPort, selfIP);
	if(bind(m_fd, (struct sockaddr *)&self_addr, sizeof(self_addr)) == -1)
	{
		throw std::runtime_error("bind failed\n");
	}

	struct sockaddr_in client_addr= GetSockaddr(portToConnect, IPtoConnect);
	if(connect(m_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
	{
		throw std::runtime_error("connection failed\n");
	}
}

UDPSocket::~UDPSocket()
{
	close(m_fd);
}

int UDPSocket::GetFD() const
{
	return m_fd;
}

void UDPSocket::Send(const char *buf, size_t len)
{
	send(GetFD(), buf, len, 0);
}


void UDPSocket::Recv(char *buf, size_t len)
{
	recv(GetFD(), buf, len, 0);
}

/******************************************************************************/
struct sockaddr_in UDPSocket::GetSockaddr(int port, const char *addr)
{
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);

    return (server_addr);
}

}//framework


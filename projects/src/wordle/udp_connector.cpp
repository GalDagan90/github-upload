#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <stdexcept>

#include "udp_connector.hpp"

namespace wordle_game
{

/*******************************************************************************
*							class UDPSender									   *
*******************************************************************************/

UDPConnector::UDPConnector (int selfPort, const char *selfIP,
							int clientPort, const char *clientIP)
													: m_sockFD{-1}
{
	m_sockFD = socket(AF_INET, SOCK_DGRAM, 0);

	struct sockaddr_in self_addr= GetSockaddr(selfPort, selfIP);
	if(bind(m_sockFD, (struct sockaddr *)&self_addr, sizeof(self_addr)) == -1)
	{
		throw std::runtime_error("bind failed\n");
	}

	struct sockaddr_in client_addr= GetSockaddr(clientPort, clientIP);
	if(connect(m_sockFD, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1)
	{
		throw std::runtime_error("connection failed\n");
	}
}

UDPConnector::~UDPConnector()
{
	close(m_sockFD);
}
	
int UDPConnector::GetFD() const
{
	return m_sockFD;
}

void UDPConnector::Send(const char *buffer, size_t len)
{
	send(GetFD(), buffer, len, 0);
}

void UDPConnector::Recv(char *buffer, size_t len)
{
	recv(GetFD(), buffer, len, 0);
}

struct sockaddr_in UDPConnector::GetSockaddr(int port, const char *addr)
{
	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(addr);

    return (server_addr);
}

} // namespace wordle_game

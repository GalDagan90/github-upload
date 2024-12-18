#ifndef __MY_WORDLE_UDP_SENDER_HPP__
#define __MY_WORDLE_UDP_SENDER_HPP__

#include <arpa/inet.h>

namespace wordle_game
{

class UDPConnector
{
public:
	UDPConnector(int selfPort, const char *selfIP, int clientPort, 
				const char *clientIP);
	~UDPConnector();
	
	int GetFD() const;
	void Send(const char *buffer, size_t len);
	void Recv(char *buffer, size_t len);

private:
	int m_sockFD;

	struct sockaddr_in GetSockaddr(int port, const char *addr);
};

} //namespace wordle_game

#endif /*__MY_WORDLE_UDP_SENDER_HPP__ */
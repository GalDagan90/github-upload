/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __UDP_CONNECTOR_H_RD5678_ILRD__
#define __UDP_CONNECTOR_H_RD5678_ILRD__

#include <boost/noncopyable.hpp> // boost::noncopyable
#include <netinet/in.h> // sockaddr_in, in_port_t
#include <sys/types.h> // protability
#include <sys/socket.h> // sockaddr

namespace framework
{

class UDPSocket : boost::noncopyable
{
public:
	UDPSocket(const char *selfIP, size_t selfPort, 
				const char *IPtoConnect, size_t portToConnect);
	~UDPSocket();

	int GetFD() const;

	void Send(const char *buf, size_t len);
	void Recv(char *buf, size_t len);

private:
	int m_fd;
	static struct sockaddr_in GetSockaddr(int port, const char *addr);
};


} // framework


#endif /* __UDP_CONNECTOR_H_RD5678_ILRD__ */

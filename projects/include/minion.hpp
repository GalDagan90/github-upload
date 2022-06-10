/****************************************************************************
*Author : Gal Dagan
*Date : 20.1.22
*Reviewer :
*
*Description : Minion Class implementation
*
*Lab : RD5678
****************************************************************************/
#ifndef __MINION_H_RD5678_ILRD__
#define __MINION_H_RD5678_ILRD__

#include "dummy_fs.hpp"
#include "udp_connector.hpp"
#include "reactor.hpp"
#include "protocol.hpp"

namespace framework
{

class Minion
{
public:
	Minion(const size_t deviceSize, const char *selfIP, size_t selfPort, 
			const char *IPtoConnect, size_t portToConnect);
	~Minion();

	void Run();

private:
	class RequestHandler;
	DummyFS m_dummy;
	UDPSocket m_udp;
	Reactor m_reactor;

	void RequestHandlerFunc(int);
};



} // namespace framework


#endif /* __MINION_H_RD5678_ILRD__ */

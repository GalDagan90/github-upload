/****************************************************************************
*Author : Gal Dagan
*Date : 20.1.22
*Reviewer :
*
*Description : Minion Class implementation
*
*Lab : RD5678
****************************************************************************/

#include "minion.hpp"

namespace framework
{

class Minion::RequestHandler
{
public:
	RequestHandler(Minion& minion);
	void operator()(int num);
private:
	Minion& m_minion;
};

Minion::RequestHandler::RequestHandler(Minion& minion)
	: m_minion(minion)
{
	//empty ctor
}

void Minion::RequestHandler::operator()(int num)
{
	m_minion.RequestHandlerFunc(num);
}

/******************************************************************************
*							Class X			    							  *
*******************************************************************************/
Minion::Minion(const size_t deviceSize, const char *selfIP, size_t selfPort, 
			const char *IPtoConnect, size_t portToConnect) :
					m_dummy(deviceSize),
					m_udp(selfIP, selfPort, IPtoConnect, portToConnect),
					m_reactor()
{
	m_reactor.Add(m_udp.GetFD(), Reactor::READ, RequestHandler(*this));
}

Minion::~Minion()
{

}

void Minion::Run()
{
	m_reactor.Run();
}

/******************************************************************************/

void Minion::RequestHandlerFunc(int)
{
	Request request;

	m_udp.Recv(reinterpret_cast<char *>(&request), sizeof(request));

	Reply reply = m_dummy.HandleRequest(request);

	m_udp.Send(reinterpret_cast<char *>(&reply), sizeof(reply));
}

} // namespace framework
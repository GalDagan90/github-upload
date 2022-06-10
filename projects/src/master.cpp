/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : NAS Master Implementation
*
*Lab : RD5678
****************************************************************************/

#include "master.hpp"

namespace framework
{


class Master::HandleRequestTask : public ThreadPool::Task
{
public:
	HandleRequestTask(Master& master, Request& request): 
		m_master(master), m_request(new Request(request))
	{
		//empty ctor
	}

	void operator()()
	{
		m_master.m_udp.Send(reinterpret_cast<char *>(m_request), sizeof(*m_request));

		Reply reply;
		m_master.m_udp.Recv(reinterpret_cast<char *>(&reply), sizeof(reply));
		m_master.m_mediator->SendReply(reply);

	}

private:
	Master& m_master;
	Request *m_request; 
};

/******************************************************************************/

Master::Master(const char *driverPath, const size_t deviceSize, const size_t threadNum,
			const char *ip, int masterPorts, int minionPorts):
				m_mediator(new InputMediator(driverPath, deviceSize, *this)),
				m_threadPool(threadNum),
				m_udp(ip, masterPorts, ip, minionPorts)
{
	
}

Master::~Master()
{
	delete m_mediator;
}

void Master::Run()
{
	m_threadPool.Run();
	m_mediator->Run();
}


void Master::HandleRequest(Request& request)
{
	boost::shared_ptr<ThreadPool::Task> reqTask(new HandleRequestTask(*this, request));
	m_threadPool.AddTask(reqTask);
}


/******************************************************************************/

}//framework

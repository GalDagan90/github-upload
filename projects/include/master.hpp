/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : NAS Master API
*
*Lab : RD5678
****************************************************************************/

#ifndef __MASTER_H_RD5678_ILRD__
#define __MASTER_H_RD5678_ILRD__

#include <iostream>
#include <vector>

#include "mediator.hpp"
#include "thread_pool.hpp"
#include "udp_connector.hpp"
#include "protocol.hpp"
#include "singleton.hpp"
#include "logger.hpp"

namespace framework
{

class InputMediator;

class Master
{
private:
	class HandleRequestTask;
public:
	/* Master(const char *driverPath, const size_t deviceSize, const size_t threadNum,
			const char *ip, int *masterPorts, int *minionPorts); */
	
	Master(const char *driverPath, const size_t deviceSize, const size_t threadNum,
			const char *ip, int masterPorts, int minionPorts);
	~Master();

	void Run();
	void HandleRequest(Request& request);

private:
	InputMediator *m_mediator;
	ThreadPool m_threadPool;
	// std::vector<boost::shared_ptr<UDPSocket> > m_udpVec;
	UDPSocket m_udp;
};




}//framework

#endif /* __MASTER_H_RD5678_ILRD__ */





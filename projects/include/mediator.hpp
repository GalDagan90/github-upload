/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : Input Mediator API
*
*Lab : RD5678
****************************************************************************/
#ifndef __MEDIATOR_H_RD5678_ILRD__
#define __MEDIATOR_H_RD5678_ILRD__

#include <boost/thread/mutex.hpp>

#include "master.hpp"
#include "reactor.hpp"
#include "nbd.hpp"

namespace framework
{

class Master;

class InputMediator
{
public:
	InputMediator(const char *driverPath, size_t deviceSize, Master& master);
	~InputMediator();

	void Run();
	void SendReply(Reply &reply);

private:
	Master& m_master;
	NBDComn m_nbdComm;
	Reactor m_reactor;
	boost::mutex m_replyMutex;
	class RequestHandler;
	void RequestHandlerFunc(int num);
};

}//framework

#endif /* __MEDIATOR_H_RD5678_ILRD__ */

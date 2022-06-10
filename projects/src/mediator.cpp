/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : Input Mediator implementation
*
*Lab : RD5678
****************************************************************************/

#include <boost/bind/bind.hpp>
#include <boost/thread/mutex.hpp>

#include "mediator.hpp"

#define UNUSED(x) ((void)(x))

namespace framework
{

class InputMediator::RequestHandler
{
public:
	RequestHandler(InputMediator& inputMediator);
	void operator()(int num);
private:
	InputMediator& m_mediator;
};

InputMediator::RequestHandler::RequestHandler(InputMediator& inputMediator)
	: m_mediator(inputMediator)
{
	//empty ctor
}

void InputMediator::RequestHandler::operator()(int num)
{
	m_mediator.RequestHandlerFunc(num);
}

/******************************************************************************/

InputMediator::InputMediator(const char *driverPath, size_t deviceSize, Master& master) : 
								m_master(master),
								m_nbdComm(driverPath, deviceSize), 
								m_reactor()
{
	m_reactor.Add(m_nbdComm.GetFD(), Reactor::READ, RequestHandler(*this));
}

InputMediator::~InputMediator()
{

}

void InputMediator::Run()
{
	m_reactor.Run();
}

void InputMediator::SendReply(Reply &reply)
{
	boost::mutex::scoped_lock lock(m_replyMutex);
	m_nbdComm.SendReply(reply);
}

void InputMediator::RequestHandlerFunc(int num)
{
	UNUSED(num);
	Request request = m_nbdComm.GetRequest();

	switch(request.type)
	{
	case Request::REQ_DISC:
		m_reactor.Stop();
		break;
	case Request::REQ_IGNORE:
		m_nbdComm.ReplyImmediately(request);
		break;
	default:
		m_master.HandleRequest(request);
		break;
	}
}

/******************************************************************************/

}//framework

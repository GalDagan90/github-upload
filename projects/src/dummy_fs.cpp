/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/

#include <cstring>
#include <iostream>

#include "dummy_fs.hpp"

namespace framework
{

DummyFS::DummyFS(size_t deviceSize) :
		m_buffFS(new char[deviceSize + 1])
{
	//empty ctor
}

DummyFS::~DummyFS()
{
	delete[] m_buffFS;
}

Reply DummyFS::HandleRequest(Request& request)
{
	if (Request::REQ_READ == request.type)
	{
		return FSRead(request);
	}
	else
	{
		return FSWrite(request);
	}
}

/******************************************************************************/

Reply DummyFS::FSRead(Request& request)
{
	Reply reply;

	reply.len = request.len;
	reply.id = request.id;
	reply.error = Reply::OK;

	boost::mutex::scoped_lock lock(m_mutex);
	memcpy(reply.buff, m_buffFS + request.from, reply.len);

	return reply;
}

Reply DummyFS::FSWrite(Request& request)
{
	Reply reply;

	reply.len = 0;
	reply.id = request.id;
	reply.error = Reply::OK;

	boost::mutex::scoped_lock lock(m_mutex);
	memcpy(m_buffFS + request.from, request.buff, request.len);

	return reply;
}


}//framework
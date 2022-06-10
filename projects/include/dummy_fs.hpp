/****************************************************************************
*Author : Gal Dagan
*Date : 
*Reviewer :
*
*Description :
*
*Lab : ol108
****************************************************************************/
#ifndef __DUMMY_FS_H_RD5678_ILRD__
#define __DUMMY_FS_H_RD5678_ILRD__

#include <boost/thread/mutex.hpp>

#include "protocol.hpp"

namespace framework
{

class DummyFS
{

public:
	DummyFS(size_t deviceSize);
	~DummyFS();

	Reply HandleRequest(Request& request);

private:
	Reply FSRead(Request& request);
	Reply FSWrite(Request& request);

	char *m_buffFS;
	boost::mutex m_mutex;
};

}//framework

#endif /* __DUMMY_FS_H_RD5678_ILRD__ */

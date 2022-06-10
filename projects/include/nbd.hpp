/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : nbd communicator API
*
*Lab : RD5678
****************************************************************************/
#ifndef __NBDComn_H_RD5678_ILRD__
#define __NBDComn_H_RD5678_ILRD__

#include <boost/noncopyable.hpp>
#include <linux/nbd.h>				//nbd_request, nbd_reply

#include "protocol.hpp"

namespace framework
{

class NBDComn : private boost::noncopyable
{
public:
	explicit NBDComn(const char *driverPath, size_t deviceSize);
	~NBDComn();
	int GetFD();

	Request GetRequest();
	void SendReply(const Reply& rep);
	void ReplyImmediately(Request &request);

private:
	int m_nbdFD;
	pid_t m_childe_pid;
	int m_socket[2];

	void CreateSocketPair(const char *driverPath, size_t deviceSize);
	void SetSignalsAndFlags();
	void SetSignalAction();
	Request TranslateRequest(const nbd_request& req);
	nbd_reply TranslateReply(const Reply& rep);
	void ReadAll(char* buf, size_t count);
	void WriteAll(const char* buf, size_t count);
	u_int64_t ntohll(u_int64_t num);

	static int nbd_dev_to_disconnect;
	static void DisconnectNbd(int signal);
	static const size_t SIZE_NBD_HANDLE = 8;
};

} // framework

#endif /* __NBDComn_H_RD5678_ILRD__ */

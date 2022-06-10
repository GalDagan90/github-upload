/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : nbd communicator
*
*Lab : RD5678
****************************************************************************/
#define _POSIX_C_SOURCE (200809L)

#include <iostream>
#include <sys/types.h>		/* fork(), u_int64_t, u_int32_t	*/
#include <sys/socket.h>		/* makesocketpair()	*/
#include <cassert>			/* assert()	*/
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>			/* sigprocmask(), sigfillset(), sigemtyset()	*/
							/*	sigaction() */
#include <sys/ioctl.h>		/* ioctl	*/
#include <sys/wait.h>		/* waitpid() */
#include <linux/nbd.h>		/* nbd_request, nbd_reply	*/
#include <netinet/in.h>		/* htonl	*/
#include <cstdlib>
#include <cstring>			/* memcpy */

#include "nbd.hpp"

#define UNUSED(x) ((void)(x))

namespace framework
{

int NBDComn::nbd_dev_to_disconnect = -1;

NBDComn::NBDComn(const char *driverPath, size_t deviceSize) : m_nbdFD(0), m_socket()
{
	CreateSocketPair(driverPath, deviceSize);

	m_childe_pid = fork();
	
	if (m_childe_pid == 0)
	{
		SetSignalsAndFlags();
	}

	SetSignalAction();
}

NBDComn::~NBDComn()
{
	int status = 0;
	ioctl(m_nbdFD, NBD_DISCONNECT);

	waitpid(m_childe_pid, &status, 0);

	close(m_nbdFD);
}

int NBDComn::GetFD()
{
	return m_socket[0];
}

Request NBDComn::GetRequest()
{
	struct nbd_request req;
	read(GetFD(), &req, sizeof(struct nbd_request));

	return TranslateRequest(req);
}

void NBDComn::SendReply(const Reply& rep)
{
	struct nbd_reply reply = TranslateReply(rep);
	WriteAll(reinterpret_cast<char *>(&reply), sizeof(reply));
	WriteAll(rep.buff, rep.len);
}

void NBDComn::ReplyImmediately(Request &request)
{
	nbd_reply reply;

	memcpy(reply.handle, &request.id, SIZE_NBD_HANDLE);
	reply.error = htonl(0);
	reply.magic = htonl(NBD_REPLY_MAGIC);
	WriteAll(reinterpret_cast<char *>(&reply), sizeof(reply));
}

/******************************************************************************/
void NBDComn::CreateSocketPair(const char *driverPath, size_t deviceSize)
{
	int err;

	err = socketpair(AF_UNIX, SOCK_STREAM, 0, m_socket);
	assert(!err);

	m_nbdFD = open(driverPath, O_RDWR);

	err = ioctl(m_nbdFD, NBD_SET_BLKSIZE, 4096);
	assert(err != -1);

	err = ioctl(m_nbdFD, NBD_SET_SIZE_BLOCKS, (deviceSize / 4096));
	assert(err != -1);

	err = ioctl(m_nbdFD, NBD_SET_TIMEOUT, 2);
	assert(err != -1);

	err = ioctl(m_nbdFD, NBD_CLEAR_SOCK);
	assert(err != -1);
}

void NBDComn::SetSignalsAndFlags()
{
	int flags;
	int err;
	sigset_t sigset;
	sigfillset(&sigset);
	sigprocmask(SIG_SETMASK, &sigset, NULL);
	
	close(m_socket[0]);

	err = ioctl(m_nbdFD, NBD_SET_SOCK, m_socket[1]);
	assert(err != -1);

	flags = 0 | NBD_FLAG_SEND_TRIM | NBD_FLAG_SEND_FLUSH;

	err = ioctl(m_nbdFD, NBD_SET_FLAGS, flags);
	assert(err != -1);

	err = ioctl(m_nbdFD, NBD_DO_IT);
	assert(err != -1);

	err = ioctl(m_nbdFD, NBD_CLEAR_QUE);
	assert(err != -1);

	err = ioctl(m_nbdFD, NBD_CLEAR_SOCK);
	assert(err != -1);

	exit(0);
}

void NBDComn::SetSignalAction()
{
	assert(nbd_dev_to_disconnect == -1);

	nbd_dev_to_disconnect = m_nbdFD;
	struct sigaction act;
	act.sa_handler = DisconnectNbd;
	act.sa_flags = SA_RESTART;
	
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGINT);
	sigaddset(&act.sa_mask, SIGTERM);
	
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);

	close(m_socket[1]);
}

Request NBDComn::TranslateRequest(const nbd_request& req)
{
	Request ret_t;

	ret_t.from = ntohll(req.from);
	ret_t.len = ntohl(req.len);
	memcpy(&ret_t.id, req.handle, SIZE_NBD_HANDLE);

	switch(ntohl(req.type))
	{
	case NBD_CMD_READ:
		ret_t.type = Request::REQ_READ;
		break;
	case NBD_CMD_WRITE:
		ReadAll(ret_t.buff, ret_t.len);
		ret_t.type = Request::REQ_WRITE;
		break;
	case NBD_CMD_DISC:
		ret_t.type = Request::REQ_DISC;
		break;
	default:
		ret_t.type = Request::REQ_IGNORE;
		break;
	}

	return ret_t;
}

nbd_reply NBDComn::TranslateReply(const Reply& rep)
{
	struct nbd_reply reply_ret;

	memcpy(reply_ret.handle, &rep.id, SIZE_NBD_HANDLE);
	switch (rep.error)
	{
	case Reply::OK:
		reply_ret.error = htonl(0);
		break;
	
	case Reply::ERROR:
		reply_ret.error = htonl(1);
		break;

	default:
		break;
	}

	reply_ret.magic = htonl(NBD_REPLY_MAGIC);

	return reply_ret;
}

void NBDComn::ReadAll(char* buf, size_t count)
{
	int bytes_read;

	while (count > 0) 
	{
		bytes_read = read(GetFD(), buf, count);
		assert(bytes_read > 0);
		buf += bytes_read;
		count -= bytes_read;
	}
	assert(count == 0);
}

void NBDComn::WriteAll(const char* buf, size_t count)
{
	int bytes_written;

	while (count > 0)
	{
		bytes_written = write(GetFD(), buf, count);
		assert(bytes_written > 0);
		buf += bytes_written;
		count -= bytes_written;
	}
	assert(count == 0);
}

u_int64_t NBDComn::ntohll(u_int64_t num)
{
	u_int32_t lo = num & 0xffffffff;
	u_int32_t hi = num >> 32U;
	lo = ntohl(lo);
	hi = ntohl(hi);
	
	return ((u_int64_t) lo) << 32U | hi;
}
/******************************************************************************/
void NBDComn::DisconnectNbd(int signal) 
{
	UNUSED(signal);
	if (nbd_dev_to_disconnect != -1) 
	{
		if(ioctl(nbd_dev_to_disconnect, NBD_DISCONNECT) == -1) 
		{
			//error
		} 
		else 
		{
			nbd_dev_to_disconnect = -1;
		}
	}
}


} //framework
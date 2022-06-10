/****************************************************************************
*Author : Gal Dagan
*Date : 13.01.22
*Reviewer : Arad Smith
*
*Description : Request and reply protocol for NBD commands
*
*Lab : RD5678
****************************************************************************/

#ifndef __ILRD_RD5678_PROTOCOL_HPP__
#define __ILRD_RD5678_PROTOCOL_HPP__

#include <sys/types.h> // u_int64_t, u_int32_t
#include <cstring>		//memcpy

namespace framework
{

const size_t PROTOCOL_BUFF_SIZE = 4096;

struct Request
{
	enum RequestType
	{
		REQ_READ,
		REQ_WRITE,
		REQ_IGNORE,
		REQ_DISC
	};

	Request();
	Request(const Request& other);

	RequestType type;
	u_int64_t id;
	u_int64_t from;
	u_int32_t len;
	char buff[PROTOCOL_BUFF_SIZE];
};

inline Request::Request()
{
    //empty ctor
}

inline Request::Request(const Request& request) : type(request.type), 
                                           id(request.id),
                                           from(request.from),
                                           len(request.len),
                                           buff("")
{
    memcpy(buff, request.buff, request.len);
}


struct Reply
{
	enum Error
	{
		OK,
		ERROR
	};

	Error error;
	u_int32_t len;
	u_int64_t id;
	char buff[PROTOCOL_BUFF_SIZE];
};

} // framework

#endif
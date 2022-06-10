#define _POSIX_C_SOURCE (200809L)

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cassert>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>


#include "nbd.hpp"
#include "reactor.hpp"


using namespace framework;

Reactor rct;

#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) {
  return a;
}
#else
u_int64_t ntohll(u_int64_t a) {
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

static void *data;
int userdata = 1;

static int xmp_read(void *buf, u_int32_t len, u_int64_t offset)
{
	fprintf(stderr, "R - %lu, %u\n", offset, len);
	memcpy(buf, (char *)data + offset, len);

	return 0;
}

static int xmp_write(const void *buf, u_int32_t len, u_int64_t offset)
{
	fprintf(stderr, "W - %lu, %u\n", offset, len);
	memcpy((char *)data + offset, buf, len);

	return 0;
}

static void xmp_disc()
{
	fprintf(stderr, "Received a disconnect request.\n");
}

static int xmp_flush()
{
	fprintf(stderr, "Received a flush request.\n");

	return 0;
}

static int xmp_trim(u_int64_t from, u_int32_t len)
{
	fprintf(stderr, "T - %lu, %u\n", from, len);

	return 0;
}

void ReactorFunc(int sk);
static int write_all(int fd, char* buf, size_t count);
static int read_all(int fd, char* buf, size_t count);

int main()
{
	const size_t DRIVE_SIZE = 1024 * 1024 * 128;
	const char DRIVE_LOCATION[] = "/dev/nbd0";
	
	NBDComn n(DRIVE_LOCATION, DRIVE_SIZE);
	
	data = malloc(DRIVE_SIZE + 1);
	if (NULL == data)
	{
		return 1;
	}

	rct.Add(n.GetFD(), framework::Reactor::READ, ReactorFunc);
	rct.Run();

	return 0;
}

void ReactorFunc(int sk)
{
	static int init = 1;
	u_int64_t from;
	u_int32_t len;
	ssize_t bytes_read;
	struct nbd_request request;
	struct nbd_reply reply;
	void *chunk;

	if (init)
	{
		reply.magic = htonl(NBD_REPLY_MAGIC);
		reply.error = htonl(0);
		init = 0;
	}

	if ((bytes_read = read(sk, &request, sizeof(request))) > 0) 
	{
		assert(bytes_read == sizeof(request));
		memcpy(reply.handle, request.handle, sizeof(reply.handle));
		reply.error = htonl(0);

		len = ntohl(request.len);
		from = ntohll(request.from);
		assert(request.magic == htonl(NBD_REQUEST_MAGIC));

		switch(ntohl(request.type)) 
		{
		case NBD_CMD_READ:
			chunk = malloc(len);
			reply.error = xmp_read(chunk, len, from);
			write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
			write_all(sk, (char*)chunk, len);

			free(chunk);
			break;
		
		case NBD_CMD_WRITE:
			chunk = malloc(len);
			read_all(sk, (char *)chunk, len);
			reply.error = xmp_write(chunk, len, from);
			free(chunk);
			write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
			break;
		
		case NBD_CMD_DISC:
			xmp_disc();
			rct.Remove(sk, framework::Reactor::READ);
			break;
		
		case NBD_CMD_FLUSH:
			xmp_flush();
			write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
			break;

		case NBD_CMD_TRIM:
			xmp_trim(from, len);
			write_all(sk, (char*)&reply, sizeof(struct nbd_reply));
			break;
		
		default:
			assert(0);
		}
	}
}

static int read_all(int fd, char* buf, size_t count)
{
	int bytes_read;

	while (count > 0) 
	{
		bytes_read = read(fd, buf, count);
		assert(bytes_read > 0);
		buf += bytes_read;
		count -= bytes_read;
	}
	assert(count == 0);

	return 0;
}

static int write_all(int fd, char* buf, size_t count)
{
	int bytes_written;

	while (count > 0)
	{
		bytes_written = write(fd, buf, count);
		assert(bytes_written > 0);
		buf += bytes_written;
		count -= bytes_written;
	}
	assert(count == 0);

	return 0;
}
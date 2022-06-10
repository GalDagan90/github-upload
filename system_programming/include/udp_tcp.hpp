#ifndef __ILRD_RD5678_UDP_TCP_H__
#define __ILRD_RD5678_UDP_TCP_H__

int CreateUDPSocket();

int CreateTCPSocket();

void InitClientStructAddr(struct sockaddr_in *addr, const int port, int s_addr);

void InitServerStructAddr(struct sockaddr_in *addr, const int port, int s_addr);

int BindServer(int sock_fd, struct sockaddr_in *addr);

int SentTo(int sock_fd, void *data, const std::size_t data_size, 
			struct sockaddr_in *recv_addr, unsigned int *addrlen);

int RecvFrom(int sock_fd, void *buffer, const std::size_t buffer_size, 
			struct sockaddr_in *recv_addr, unsigned int *addrlen);


#endif /* */
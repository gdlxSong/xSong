#include"core.h"
#include"setSock.h"



bool gdl::setnonblocking(int fd, bool flag) {

	int fmode = 0;
	if (-1 == (fmode = fcntl(fd, F_GETFL, 0))) {
		return false;
	}

	if (flag)
		fmode &= (~O_NONBLOCK);
	else
		fmode |= O_NONBLOCK;

	if (-1 == fcntl(fd, F_SETFL, fmode))
		return false;
	return true;
}


void gdl::initsockaddr(sockaddr_in& saddr, sa_family_t family, unsigned short port, char* addr) {

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(addr);
}

void gdl::initsockaddr(sockaddr_in& saddr, sa_family_t family, unsigned short port, in_addr_t addr) {

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = addr;
}


int gdl::accept(int __fd, __SOCKADDR_ARG __addr,
	socklen_t* __addr_len) {


	do {
		//accept. 
		int fd = ::accept(__fd, __addr, __addr_len);
		if (SOCKET_ERROR == fd) {
			//EINTR.
			if (errno == EINTR) {
				//some interrupt.
				continue;
			}
			//EAGAIN or EWOULDBLOCK
			if (EWOULDBLOCK == errno || EAGAIN == errno) {
				// this non-block behive.
				return SOCKET_NONBLOCK;
			}
			else
				return SOCKET_ERROR;
		}
		return fd;
	} while (true);

}
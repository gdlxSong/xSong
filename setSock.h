#ifndef SETSOCK_H_
#define SETSOCK_H_

#include"core.h"


namespace gdl {

	//
	bool setnonblocking(int fd, bool flag);
	//
	void initsockaddr(sockaddr_in& saddr, sa_family_t family, unsigned short port, char* addr);
	//
	void initsockaddr(sockaddr_in& saddr, sa_family_t family, unsigned short port, in_addr_t addr);
	//
	int accept(int __fd, __SOCKADDR_ARG __addr, socklen_t* __addr_len);
	//

}




#endif
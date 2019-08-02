/***********************************************************

==关于我对accept模块的设计：
==
	0. 使用epoll是必须的
	1. 使用主线程对epoll中所有注册的fd进行监控.
	2. 然后将活动的fd移交给工作线程或者进程
	3. 注意异常的处理


***********************************************************/


//建立主线程，内置epoll，注册fd，监听fd活动，然后进行分派



#include"core.h"
#include"setSock.h"
#include<string.h>
#include<stdlib.h>
#include<iostream>

//define socket == fd
int fd_serv, fd_cli;

//define epoll event.
struct epoll_event ev, events[MAX_EPOLL_EVENTS];

//define number waitable socket.
int nfds;




char* response = "HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nContent-Length:14\r\n\r\n<h1>hello</h1>";

void do_use_fd(int fd) {


	send(fd, response, strlen(response), 0);
	close(fd);
}


int main(int argc, char** argv) {


	//get host port.
	short PORT = 9999;
	if (argc == 2) {
		PORT = atoi(argv[1]);
	}
	fd_serv = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == fd_serv) {

		std::cout << "call socket faild." << std::endl;
		return SOCKET_ERROR;
	}

	//set fd_serv non-block.
	gdl::setnonblocking(fd_serv, false);

	//set reuse addr for quickly restart process.
	int val = 1;
	if (setsockopt(fd_serv, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) < 0) {
		std::cout << "set address reuse faild." << std::endl;
	}

	sockaddr_in local = { 0 };
	socklen_t addrlen = sizeof(local);
	sockaddr_in saddr;

	//initial socket address.
	gdl::initsockaddr(saddr, AF_INET, PORT, "0.0.0.0");

	//bind socket with sockaddress.
	int ret = bind(fd_serv, (sockaddr*)& saddr, sizeof(saddr));
	if (0 != ret)
	{
		std::cout << "failed to call bind" << std::endl;
		return SOCKET_ERROR;
	}

	//listen socket.
	listen(fd_serv, 20);

	//create a epoll as int.
	int epollfd = epoll_create(10);
	if (epollfd == SOCKET_ERROR) {
		std::cout << "create epoll handler faild." << std::endl;
		exit(EXIT_FAILURE);
	}

	//add fd_serv to epoll.
	ev.events = EPOLLIN;
	ev.data.fd = fd_serv;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd_serv, &ev) == SOCKET_ERROR) {
		perror("epoll_ctl: listen_sock");
		exit(EXIT_FAILURE);
	}

	for (;;) {
		nfds = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
		if (nfds == SOCKET_ERROR) {
			//if errno is EINTR
			if (EINTR == errno) {
				continue;
			}
			else {
				//else, exit.
				std::cout << "call epoll_wait faild." << std::endl;
				return(EXIT_FAILURE);			
			}
		}

		for (int n = 0; n < nfds; n++) {
			//if this event active is from fd_serv. accept.
			if (events[n].data.fd == fd_serv) {
				fd_cli = gdl::accept(fd_serv, (struct sockaddr*) & local, &addrlen);

				if (SOCKET_ERROR == fd_cli) {
					//some errno occurred.
					std::cout << "called accept faild." << std::endl;
					exit(EXIT_FAILURE);
				}
				if (SOCKET_NONBLOCK == fd_cli) {
					//non-block socket.
					continue;
				}
				//set fd_cli non-block.
				gdl::setnonblocking(fd_cli, true);
				//chooice ET-epoll, and add fd.
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = fd_cli;
				if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd_cli, &ev) == SOCKET_ERROR) {
					std::cout << "called epoll_ctl faild." << std::endl;
					//close fd_cli and continue. default delete fd from epoll.
					close(fd_cli);
					//exit(EXIT_FAILURE);
				}
			}
			else {
				//dispatch fd.
				do_use_fd(events[n].data.fd);
			}
		}
	}
	return 0;

}
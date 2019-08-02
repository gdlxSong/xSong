#pragma once

#ifndef CORE_H_
#define CORE_H_



/*����ĵ���������������������ص����к���������ļ�.*/

#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/********************************************************
==
==˳�㶨��һЩ����.
*********************************************************/

//define 64K buffer size
#define BUF_SIZE 65535	

//define SOCKET_ERROR
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

//number of events.
#define MAX_EPOLL_EVENTS 10

//flag of non-block.
#define SOCKET_NONBLOCK -2

#endif
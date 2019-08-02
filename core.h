#pragma once

#ifndef CORE_H_
#define CORE_H_



/*这个文档用来包含服务器开发相关的所有核心网络库文件.*/

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
==顺便定义一些常量.
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
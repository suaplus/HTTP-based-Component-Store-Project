#ifndef __SER_TCP_INIT_H__
#define __SER_TCP_INIT_H__

#define SER_PORT 80
#define MAX_CLI_NUM 100

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

extern int ser_tcp_init();

#endif
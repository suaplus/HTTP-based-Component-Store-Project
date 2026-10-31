#ifndef ___SEND_INFO_H__
#define __SEND_INFO_H__ 

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

extern int send_head(int conn, char *filename);
extern int send_file(int connfd, char *name);

#endif
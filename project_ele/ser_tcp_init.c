#include "ser_tcp_init.h"

int ser_tcp_init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ser tcp init  socket error");
        return -1;
    }

    struct sockaddr_in seraddr;
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(SER_PORT);
    seraddr.sin_addr.s_addr = INADDR_ANY;
    socklen_t slen = sizeof(seraddr);

    int ret = bind(sockfd, (struct sockaddr *)&seraddr, slen);
    if (ret < 0)
    {
        perror("ser tcp init bind error");
        return -1;
    }

    ret = listen(sockfd, MAX_CLI_NUM);
    if (ret < 0)
    {
        perror("ser tcp init listen error");
        return -1;
    }

    return sockfd;
}
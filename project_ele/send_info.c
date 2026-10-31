#include "send_info.h"
int send_head(int conn, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (NULL == fp)
    {
        perror("send_head fopen");
        return 1;
    }
    fseek(fp, 0, SEEK_END);

    long size = ftell(fp);
    fclose(fp);
    char *http_cmd[6] = {NULL};
    char temp[128] = {0};
    http_cmd[0] = "HTTP/1.1 200 OK\r\n";
    http_cmd[1] = "Date: Mon, 07 Sep 2026 07:57:49 GMT\r\n";
    http_cmd[2] = "Content-Type: text/html;charset=utf-8\r\n";
    http_cmd[3] = "Server: PTSERVER\r\n";
    http_cmd[4] = temp;
    sprintf(temp, "Content-Length: %ld\r\n", size);
    http_cmd[5] = "Connection: closed\r\n\r\n";
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        send(conn, http_cmd[i], strlen(http_cmd[i]), 0);
    }
    return 0;
}


int send_file(int connfd, char *name)
{
    int fd = open(name, O_RDONLY);
    if(fd < 0)
    {
        perror("send file read error");
        return -1;
    }
    
    char buff[1024] = {0};
    int ret_fd = 0;
    
    while(1)
    {
        memset(buff, 0, sizeof(buff));
        ret_fd = read(fd, buff, sizeof(buff));
        if(ret_fd <= 0)
        {
            break;
        }
        send(connfd, buff, ret_fd, 0);
    }

    close(fd);

    return 0;
}
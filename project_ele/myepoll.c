#include "myepoll.h"
#include "send_info.h"
#include "ser_tcp_init.h"
#include "user_ctl.h"
#include "url2.h"
int add_epoll_fd(int epfds, int sockfd, uint32_t event)
{
	struct epoll_event ev;
	ev.events = event;
	ev.data.fd = sockfd;

	int ret = epoll_ctl(epfds, EPOLL_CTL_ADD, sockfd, &ev);
	if(ret < 0)
	{
		perror("epoll ctl add error");
		return -1;
	}

	return 0;
}
int del_epoll_fd(int epfds, int sockfd)
{
	int ret = epoll_ctl(epfds, EPOLL_CTL_DEL, sockfd, NULL);
	if(ret < 0)
	{
		perror("epoll ctl del error");
		return -1;
	}

	return 0;
}

int epoll_server()
{
	int sockfd = ser_tcp_init();
	if(sockfd < 0)
	{
		perror("ser init error");
		return -1;
	}

	int epfds = epoll_create(MAX_CLI_NUM);
	if(epfds < 0)
	{
		perror("epoll dreate error");
		return -1;
	}

	int ret = add_epoll_fd(epfds, sockfd, EPOLLIN);
	if(ret < 0)
	{
		perror("add epoll fd error");
		return -1;
	}

	struct epoll_event evs[MAX_CLI_NUM];
	struct sockaddr_in cliaddr;
	socklen_t clen = sizeof(cliaddr);

	while(1)
	{
		int cnt = epoll_wait(epfds, evs, MAX_CLI_NUM, -1);
		if(cnt < 0)
		{
			perror("epoll wait error");
			return -1;
		}

		for(int i = 0; i < cnt; ++i)
		{
            int connfd = 0;
			if(evs[i].data.fd == sockfd)
			{
				connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clen);
				if(connfd < 0)
				{
					perror("accept error");
					return -1;
				}

				add_epoll_fd(epfds, connfd, EPOLLIN);
			}else
			{
				char buff[1024] = {0};
                char *method = strtok(buff, " ");
                char *url = strtok(NULL, " ");
                char *vision = strtok(NULL, "\r");

                if(0 == strcmp(url, "/"))
                {
                    send_file(connfd, "./0.html");
                }
                else if(0 == strcmp(url, "/register"))
                {
                    send_file(connfd, "./1.html");
                }
                ///127.0.0.1?%E7%94%A8%E6%88%B7%E5%90%8D=lisi&%E5%AF%86%E7%A0%81=111
                else if(0 == strncmp(url, "/register_sub", 13))
                {
                    //send_file(connfd, "./1.html");

                    char *name_r = NULL;
                    char *pass_r = NULL;
                    char *end_r = NULL;
                    name_r = index(url, '=');  // strchr
                    name_r += 1;
                    end_r = index(name_r, '&');
                    *end_r = '\0';

                    pass_r = index(end_r + 1, '=');
                    pass_r += 1;
                    printf("name_r = %s, pass_r = %s\n", name_r, pass_r);
                    if(0 == user_register(name_r, pass_r))
                    {
                        send_file(connfd, "./4.html");
                    }
                    
                }else if(0 == strncmp(url, "/login", 6))
                {
                    char *name = NULL;
                    char *pass = NULL;
                    char *end = NULL;
                    name = index(url, '=');  // strchr
                    name += 1;
                    end = index(name, '&');
                    *end = '\0';

                    pass = index(end + 1, '=');
                    pass += 1;

                    if (0 == user_login(name, pass))
                    {
                        send_file(connfd, "./2.html");
                    }
                    else
                    {
                        send_file(connfd, "./3.html");
                    }
                }else if(0 == strncmp(url, "/s", 2))
                {
                    char *name_ele = url;
                    name_ele += 2;
                    if(0 == strcmp(name_ele, "zlc.html"))
                    {
                        send_file(connfd, "./zlc.html");
                    }else if(0 == strcmp(name_ele, "plc.html"))
                    {
                        send_file(connfd, "./plc.html");
                    }
                }else if(0 == strncmp(url, "/9", 2))
                {
                    char *name_ele = url;
                    name_ele += 2;
                    if(0 == strcmp(name_ele, "001.html"))
                    {
                        send_file(connfd, "./9001.html");
                    }else if(0 == strcmp(name_ele, "002.html"))
                    {
                        send_file(connfd, "./9002.html");
                    }
                }else if(0 == strncmp(url, "/find", 5))
                {
                    char *name_fd = index(url, '=');  // strchr
                    name_fd += 1;

                    urldecode(name_fd);
                    if(0 == strcmp(name_fd, "铝电解电容器"))
                    {
                        send_file(connfd, "./zlc.html");
                    }else if(0 == strcmp(name_fd, "贴片型铝电解电容"))
                    {
                        send_file(connfd, "./plc.html");
                    }
                }
                else if (strlen(url) > 4 && 0 == strcmp(&url[strlen(url) - 4], ".jpg"))
                {
                    send_file(connfd, url + 1);
                }
                else if (strlen(url) > 4 && 0 == strcmp(&url[strlen(url) - 4], ".png"))
                {
                    send_file(connfd, url + 1);
                }
                else if (0 == strcmp(url, "/favicon.ico"))
                {
                    send_file(connfd, "0.png");
                }
                close(connfd);
			}
		}
	}
	return 0;
}
#include "send_info.h"
#include "ser_tcp_init.h"
#include "user_ctl.h"
#include "url2.h"

int main()
{
    // user_init();

    int sockfd = ser_tcp_init();

    struct sockaddr_in cliaddr;
    socklen_t clen = sizeof(cliaddr);

    while (1)
    {
        int connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clen);
        if (connfd < 0)
        {
            perror("accept error");
            return -1;
        }

        char buff[1024] = {0};

        memset(buff, 0, sizeof(buff));
        ssize_t size = recv(connfd, buff, sizeof(buff), 0);
        if (size <= 0)
        {
            // perror("recv error");
            close(connfd);
            continue;
        }

        printf("%s\n", buff);
        // GET /127.0.0.1?%E7%94%A8%E6%88%B7%E5%90%8D=111&%E5%AF%86%E7%A0%81=222 HTTP/1.1
        char *method = strtok(buff, " ");
        char *url = strtok(NULL, " ");
        char *vision = strtok(NULL, "\r");

        if (0 == strcmp(url, "/"))
        {
            send_file(connfd, "./0.html");
        }
        else if (0 == strcmp(url, "/register"))
        {
            send_file(connfd, "./1.html");
        }
        /// 127.0.0.1?%E7%94%A8%E6%88%B7%E5%90%8D=lisi&%E5%AF%86%E7%A0%81=111
        else if (0 == strncmp(url, "/register_sub", 13))
        {
            // send_file(connfd, "./1.html");

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
            if (0 == user_register(name_r, pass_r))
            {
                send_file(connfd, "./4.html");
            }
        }
        else if (0 == strncmp(url, "/login", 6))
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
        }
        else if (0 == strncmp(url, "/s", 2))
        {
            char *name_ele = url;
            name_ele += 2;
            if (0 == strcmp(name_ele, "zlc.html"))
            {
                send_file(connfd, "./zlc.html");
            }
            else if (0 == strcmp(name_ele, "plc.html"))
            {
                send_file(connfd, "./plc.html");
            }
        }
        else if (0 == strncmp(url, "/9", 2))
        {
            char *name_ele = url;
            name_ele += 2;
            if (0 == strcmp(name_ele, "001.html"))
            {
                send_file(connfd, "./9001.html");
            }
            else if (0 == strcmp(name_ele, "002.html"))
            {
                send_file(connfd, "./9002.html");
            }
        }
        else if (0 == strncmp(url, "/find", 5))
        {
            char *name_fd = index(url, '=');  // strchr
            name_fd += 1;

            urldecode(name_fd);
            if (0 == strcmp(name_fd, "铝电解电容器"))
            {
                send_file(connfd, "./zlc.html");
            }
            else if (0 == strcmp(name_fd, "贴片型铝电解电容"))
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

    close(sockfd);

    return 0;
}

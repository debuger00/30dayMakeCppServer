#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> // bzero 函数
#include <unistd.h> // 标准 输入输出， 提供了 对POSIX   系统
#include "util.h"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error"); // SOMAXCONN 是 listen() 函数中用于设置 ​监听队列最大长度 的系统级常量 linux 默认值是 128

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof((clnt_addr));
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    errif(clnt_sockfd == -1, "socket accept error");

    printf(" 新客户端文件描述符 %d  IP: %s  端口号: %d \n ", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("来自客服端fd %d的消息 ：  %s \n ", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            close(clnt_sockfd);
            errif(clnt_sockfd, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}
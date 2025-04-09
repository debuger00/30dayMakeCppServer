#include <stdio.h>
#include <sys/socket.h> // 套接字
#include <arpa/inet.h>  // 地址转换 ， eg inet_pton()和 inet_ntop()
#include <string.h>     // 字符串处理  strcpy、strlen 和 strcmp
#include <unistd.h>     // 标准输入输出 提供了对POSIX操作系统API的访问，例如文件操作、进程控制等。包含了 read、write、close 等系统调用。
#include "util.h"

int main()
{ // 服务端创建了两个socket对象，第一个是用于监听连接请求，第二个是接收到的套接字描述符，用于收发数据。listen()时正式启动socket。

    printf("当前进程 PID = %d\n", getpid());
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // 第一个是用于监听连接请求，
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "socket bind error");

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len); // wyh  接受连接请求， 返回新的socket套接字文件描述符 //wyh 第二个是接收到的套接字描述符，用于收发数据。listen()时正式启动socket。
    errif(clnt_sockfd == -1, "socket accept error");

    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    while (true)
    {
        char buf[1024];
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));// 服务器 先读后写
        if (read_bytes > 0)
        {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf));
        }
        else if (read_bytes == 0)
        {
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }
        else if (read_bytes == -1)
        {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}

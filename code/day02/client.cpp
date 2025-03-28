#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // 创建套接字
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr; // 创建 套接字 结构体 struct sockaddr_in
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error"); // connect

    while (true) // 死循环
    {
        // 创建一个1024字节的缓冲区用于存储数据
        char buf[1024];
        // 将缓冲区清零
        bzero(&buf, sizeof(buf));
        // 从标准输入读取用户输入的字符串
        scanf("%s", buf);
        // 将用户输入的数据写入socket
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf)); //       ############################### 这里很重要 原来 scanf 除了 手工输入， 还可以 从 标准输读取数据
        // 如果写入失败,说明socket已断开连接
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        // 清空缓冲区,准备接收服务器的响应
        bzero(&buf, sizeof(buf));
        // 从socket读取服务器发来的数据
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        // 如果读取到数据,打印服务器发来的消息
        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        // 如果读取到0字节,说明服务器已关闭连接
        else if (read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }
        // 如果读取出错,关闭socket并报错
        else if (read_bytes == -1)
        {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}

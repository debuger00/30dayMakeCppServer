#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

int main()
{
    int sockefd = socket(AF_INET, SOCK_STREAM, 0); // 创建套接字

    // 创建 套接字 结构体 struct sockaddr_in
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    // connect
    errif(connect(sockefd, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1, "sockect connect error");

    while (true)
    {
        char buf[1024];
        bzero(buf, sizeof(buf));
        scanf("%s", buf);

        ssize_t write_bytes = write(sockefd, buf, 1024);

        if (write_bytes == -1)
        {
            printf("socket already disconnect,can't write any more! \n");
            break;
        }

        bzero(&buf, sizeof(buf));

        ssize_t read_bytes = read(sockefd, buf, sizeof(buf));

        if (read_bytes > 0)
        {
            printf("message from server: %s", buf);
        }
        else if (read_bytes == 0)
        {
            printf("disconnecte");
        }
        else if (read_bytes == -1)
        {
            printf("message disconnect ");
            break;
        }
    }
    close(sockefd);

    // 死循环
    // 创建一个1024字节的缓冲区用于存储数据
    // 将缓冲区清零
    // 从标准输入读取用户输入的字符串
    // 如果写入失败,说明socket已断开连接
    // 清空缓冲区,准备接收服务器的响应
    // 从socket读取服务器发来的数据
    // 如果读取到数据,打印服务器发来的消息
    // 如果读取到0字节,说明服务器已关闭连接
    // 如果读取到0字节,说明服务器已关闭连接
    // 如果读取出错,关闭socket并报错
}
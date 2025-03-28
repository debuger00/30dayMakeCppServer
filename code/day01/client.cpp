/*
 * @Author: your name
 * @Date: 2022-01-04 20:03:45
 * @LastEditTime: 2022-01-05 19:08:58
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \30dayMakeCppServer\code\day01\client.cpp
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    // 创建一个socket文件描述符，使用TCP/IP协议
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 创建并初始化服务器地址结构体
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));               // 将结构体清零
    serv_addr.sin_family = AF_INET;                     // 使用IPv4协议
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置服务器IP地址为本机
    serv_addr.sin_port = htons(8888);                   // 设置服务器端口为8888

    // bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    //  客户端不需要绑定端口,系统会自动分配一个端口

    // 连接到服务器
    connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));

    // 发送测试信息给服务器
    const char *message = "Hello, Server!";
    send(sockfd, message, strlen(message), 0);
    send(sockfd, message, strlen(message), 0);

    // 关闭socket
    close(sockfd);
    return 0;
}

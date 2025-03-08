#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    // 创建一个socket文件描述符，使用TCP/IP协议
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    // 创建并初始化服务器地址结构体
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));               // 将结构体清零
    serv_addr.sin_family = AF_INET;                     // 使用IPv4协议
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 设置服务器IP地址为本机
    serv_addr.sin_port = htons(8888);                   // 设置服务器端口为8888

    // 将socket文件描述符和服务器地址结构体绑定
    if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    // 开始监听socket，等待客户端连接请求
    if (listen(sockfd, SOMAXCONN) < 0) {
        perror("listen failed");
        return 1;
    }

    // 创建并初始化客户端地址结构体，用于存储连接上的客户端信息
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    // 接受客户端连接请求，返回一个新的socket文件描述符用于和该客户端通信
    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    if (clnt_sockfd < 0) {
        perror("accept failed");
        return 1;
    }

    // 打印连接上的客户端信息(文件描述符、IP地址和端口号)
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    fflush(stdout); // 确保输出立即显示

    return 0;
}

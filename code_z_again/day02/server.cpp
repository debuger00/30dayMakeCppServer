#include <sys/socket.h>
#include <netinet/in.h> // 包含 struct sockaddr_in
#include <arpa/inet.h>  // 包含inet_addr和  htons
#include "util.h"
#include <stdio.h>  // 包含printf函数
#include <unistd.h> //包含read , write 函数
#include <string.h> // 包含 memset 函数

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8989);
    bind(listen_fd, (sockaddr *)&server_addr, sizeof(server_addr));
    listen(listen_fd, SOMAXCONN); // 这里的somaxconn是什么？ // somaxconn设置连接请求队列的长度
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    // accept(listen_fd, (sockaddr *)&client_addr, (socklen_t *)&sizeof(client_addr)); // 这里和bind的不一样, sizeof(client_addr)是左值，不能取地址
    printf("accept正在阻塞");
    int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &client_addr_len); // 这里和bind的不一样,
    printf("accept阻塞结束");
    // printf("客户端的fd为: %d, 客户端的IP地址为: %c, 客户端的端口号为:%d", client_fd, client_addr.sin_addr.s_addr, client_addr.sin_port);
    printf("客户端的fd为: %d, 客户端的IP地址为: %s, 客户端的端口号为:%d", client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    while (true)
    {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        ssize_t read_bit = read(client_fd, buf, sizeof(buf));

        // 检查读取是否出错或连接是否已关闭
        if (read_bit <= 0)
        {
            if (read_bit == 0)
            {
                printf("客户端已断开连接\n");
            }
            else
            {
                printf("读取错误\n");
            }
            break; // 退出循环
        }

        printf("客户端发来的消息为: %s\n", buf);

        memset(buf, 0, sizeof(buf));
        strcpy(buf, "服务器回应了wyh");
        size_t write_bit = write(client_fd, buf, sizeof(buf));
    };

    // 关闭客户端socket
    close(client_fd);
}
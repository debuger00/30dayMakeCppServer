#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

// 客户端
int main()
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8989);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(client_fd, (sockaddr *)&server_addr, sizeof(server_addr));
    while (true)
    {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        scanf("%s", buf);

        ssize_t write_bit = write(client_fd, buf, sizeof(buf));
        if (write_bit <= 0)
        {
            printf("发送数据失败\n");
            break;
        }

        memset(buf, 0, sizeof(buf));
        ssize_t read_bit = read(client_fd, buf, sizeof(buf));
        if (read_bit <= 0)
        {
            printf("接收数据失败或服务器已断开连接\n");
            break;
        }

        printf("服务器响应： %s\n", buf);
    }

    // 关闭socket
    close(client_fd);
};
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
        write(client_fd, buf, sizeof(buf));
        memset(buf, 0, sizeof(buf));
        read(client_fd, buf, sizeof(buf));
        printf("服务器响应： %s \n", buf);
    }
};
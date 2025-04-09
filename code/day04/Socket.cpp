#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd(-1)
{ // 创建新的TCP/IPv4流式套接字

    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}
Socket::Socket(int _fd) : fd(_fd)
{ // 这里的fd(_fd) 就相当于给fd 成员变量初始化
    errif(fd == -1, "socket create error");
}

Socket::~Socket()
{ // 析构函数
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *addr)
{ // 绑定
    errif(::bind(fd, (sockaddr *)&addr->addr, addr->addr_len) == -1, "socket bind error");
}

void Socket::listen()
{
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}
void Socket::setnonblocking()
{ // 设置 为非阻塞模式
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr)
{
    int clnt_sockfd = ::accept(fd, (sockaddr *)&addr->addr, &addr->addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    return clnt_sockfd;
}

int Socket::getFd()
{
    return fd;
}
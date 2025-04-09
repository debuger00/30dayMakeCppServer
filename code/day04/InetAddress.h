#pragma once
#include <arpa/inet.h>

class InetAddress
{
public:
    struct sockaddr_in addr; // 套接字IP地址 IPv4地址和端口的套接字地址结构体
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress();
};

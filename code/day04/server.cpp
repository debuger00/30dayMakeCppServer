#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include "util.h"
#include "Epoll.h"
#include "InetAddress.h"
#include "Socket.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
void handleReadEvent(int);

int main()
{
    Socket *serv_sock = new Socket();                            // wyh 注意这个 serv_sock是 指针  //wyh 创建socket套接字
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888); // wyh 创建套接字地址结构体
    serv_sock->bind(serv_addr);                                  // wyh socket套接字绑定 上述套接字地址
    serv_sock->listen();                                         // wyh socket 套接字监听
    serv_sock->setnonblocking();                                 // wyh  serv_sock 设置为非阻塞模式
    Epoll *ep = new Epoll();                                     // wyh 创建epoll类
    ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);            // wyh  讲serv的socket 绑定到 上一步的epoll类中
    while (true)
    {
        std::vector<epoll_event> events = ep->poll(); // 事件等待S
        int nfds = events.size();                     // nfds 表示就绪事件的数量
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == serv_sock->getFd())
            {                                                                 // 新客户端连接
                InetAddress *clnt_addr = new InetAddress();                   // 会发生内存泄露！没有delete
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr)); // 会发生内存泄露！没有delete
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
                clnt_sock->setnonblocking();
                ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);
            }
            else if (events[i].events & EPOLLIN)
            { // 可读事件
                handleReadEvent(events[i].data.fd);
            }
            else
            { // 其他事件，之后的版本实现
                printf("something else happened\n");
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int sockfd)
{
    char buf[READ_BUFFER];
    while (true)
    { // 由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0)
        {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        }
        else if (bytes_read == -1 && errno == EINTR)
        { // 客户端正常中断、继续读取
            printf("continue reading");
            continue;
        }
        else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
        { // 非阻塞IO，这个条件表示数据全部读取完毕
            printf("finish reading once, errno: %d\n", errno);
            break;
        }
        else if (bytes_read == 0)
        { // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd); // 关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
}

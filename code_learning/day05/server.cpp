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
#include "Channel.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
void handleReadEvent(int);

int main()
{
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();
    Channel
}
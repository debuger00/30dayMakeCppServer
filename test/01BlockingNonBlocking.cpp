#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // 创建一个命名管道（FIFO）
    system("mkfifo testfifo");

    int fd = open("testfifo", O_RDONLY); // 打开FIFO
    char buf[1024];
    printf("等待数据...\n");
    ssize_t n = read(fd, buf, sizeof(buf)); // 这里会真正阻塞，直到有数据写入
    printf("读取到数据，n: %ld\n", n);
    close(fd);

    unlink("testfifo"); // 删除FIFO
    return 0;
}
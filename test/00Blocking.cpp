#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <sys/wait.h>

int main()
{
    // const char *fifo_path = "/mnt/e/program/C++/C++_project/30dayMakeCppServer/test/file";
    // const char *fifo_path = "/mnt/e/program/C++/C++_project/30dayMakeCppServer/test/file.txt";
    const char *fifo_path = "/tmp/myfifo";
    mkfifo(fifo_path, 0666); // 创建命名管道

    // pid_t pid = fork();
    // if (pid == 0)
    // {             // 子进程（写入端）
    //     sleep(3); // 模拟延迟
    //     int fd = open(fifo_path, O_WRONLY);
    //     std::cout << "子进程开始写入...\n";
    //     write(fd, "Hello", 5);
    //     close(fd);
    //     return 0;
    // }

    // 父进程（读取端）
    int fd = open(fifo_path, O_RDONLY);
    std::cout << "父进程开始读取...\n";

    char buf[1024];
    ssize_t n = read(fd, buf, sizeof(buf)); // 这里会阻塞3秒
    buf[n] = '\0';

    std::cout << "收到 " << n << " 字节: " << buf << std::endl;
    close(fd);
    wait(nullptr);
    // unlink(fifo_path); // 清理管道文件
}
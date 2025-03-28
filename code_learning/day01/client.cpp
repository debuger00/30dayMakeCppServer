/*
 * @Author: your name
 * @Date: 2022-01-04 20:03:45
 * @LastEditTime: 2022-01-05 19:08:58
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \30dayMakeCppServer\code\day01\client.cpp
 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main()
{
   int sockfd = socket(AF_INET, SOCK_STREAM, 0);

   struct sockaddr_in serv_addr;
   bzero(&serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   serv_addr.sin_port = htons(8888);

   connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr));

   const char *message = "Hello,Server!";
   send(sockfd, message, strlen(message), 0);

   close(sockfd);

   return 0;
}

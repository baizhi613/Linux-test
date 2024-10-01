#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Thread.hpp"
#include "Comm.hpp"

using namespace ThreadModule;

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " serverip serverport\n"
              << std::endl;
}

int InitClient(std::string &serverip, uint16_t serverport, struct sockaddr_in *server)
{
    // 1. 创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        return -1;
    }
    // 2. client要不要bind？一定要，client也要有自己的IP和PORT。要不要显式[和server一样用bind函数]的bind？不能！不建议！！
    // a. 如何bind呢？udp client首次发送数据的时候，OS会自己自动随机的给client进行bind --- 为什么？防止client port冲突。要bind，必然要和port关联！
    // b. 什么时候bind呢？首次发送数据的时候

    // 构建目标主机的socket信息

    memset(server, 0, sizeof(struct sockaddr_in));
    server->sin_family = AF_INET;
    server->sin_port = htons(serverport);
    server->sin_addr.s_addr = inet_addr(serverip.c_str());

    return sockfd;
}

void recvmessage(int sockfd, std::string name)
{
    // int fd = OpenDev("/dev/pts/2", O_WRONLY);
    while (true)
    {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        char buffer[1024];
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
        if (n > 0)
        {
            buffer[n] = 0;
            fprintf(stderr, "[%s]%s\n", name.c_str(), buffer); // stderr
            //write(fd, buffer, strlen(buffer));
        }
    }
}
void sendmessage(int sockfd, struct sockaddr_in &server, std::string name)
{
    std::string message;
    while (true)
    {
        printf("%s | Enter# ", name.c_str());
        fflush(stdout);
        std::getline(std::cin, message);
        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));
    }
}

// ./udpclient serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);
    struct sockaddr_in serveraddr;
    int sockfd = InitClient(serverip, serverport, &serveraddr);
    if (sockfd == -1)
        return 1;

    func_t r = std::bind(&recvmessage, sockfd, std::placeholders::_1);
    func_t s = std::bind(&sendmessage, sockfd, serveraddr, std::placeholders::_1);

    Thread Recver(r, "recver");
    Thread Sender(s, "sender");

    Recver.Start();
    Sender.Start();

    Recver.Join();
    Sender.Join();

    return 0;
}
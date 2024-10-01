#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " serverip serverport\n"
              << std::endl;
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

    // 1. 创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
    }
    // 2. client要不要bind？一定要，client也要有自己的IP和PORT。要不要显式[和server一样用bind函数]的bind？不能！不建议！！
    // a. 如何bind呢？udp client首次发送数据的时候，OS会自己自动随机的给client进行bind --- 为什么？防止client port冲突。要bind，必然要和port关联！
    // b. 什么时候bind呢？首次发送数据的时候

    // 构建目标主机的socket信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    std::string message;
    // 2. 直接通信即可
    while(true)
    {
        std::cout << "Please Enter# ";
        std::getline(std::cin, message);
        sendto(sockfd, message.c_str(), message.size(), 0, (struct sockaddr*)&server, sizeof(server));

        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        char buffer[1024];
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&peer, &len);
        if(n > 0)
        {
            buffer[n] = 0;
            std::cout << "server echo# " << buffer << std::endl;
        }
    }
    return 0;
}
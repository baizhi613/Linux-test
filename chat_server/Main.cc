#include <iostream>
#include <memory>
#include "MessageRoute.hpp"
#include "UdpServer.hpp"

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " local_port\n" << std::endl;
}
// using func_t = std::function<std::string(int sockfd, const std::string message, const InetAddr who)>; // TODO

// ./udpserver port
// 云服务器的port默认都是禁止访问的。云服务器放开端口8080 ~ 8085
int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERROR);
    }
    EnableScreen();
    // std::string ip = argv[1]; // TODO
    // 定义消息转发模块
    MessageRoute route;

    // 网络部分
    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port,\
        std::bind(&MessageRoute::Route, &route, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)); // C++14
    usvr->InitServer();
    usvr->Start();

    return 0;
}
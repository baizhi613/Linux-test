#pragma once

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <functional>
#include "Log.hpp"
#include "InetAddr.hpp"

// echo server -> client -> server
enum
{
    SOCKET_ERROR = 1,
    BIND_ERROR,
    USAGE_ERROR
};

const static int defaultfd = -1;

// typedef std::function<std::string(const std::string&)> func_t;
using func_t = std::function<std::string(const std::string&, bool &ok)>;

class UdpServer
{
public:
    UdpServer(uint16_t port, func_t func) : _sockfd(defaultfd), _port(port), _isrunning(false), _func(func)
    {
    }
    void InitServer()
    {
        // 1. 创建udp socket 套接字 --- 必须要做的
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockfd < 0)
        {
            LOG(FATAL, "socket error, %s, %d\n", strerror(errno), errno);
            exit(SOCKET_ERROR);
        }
        LOG(INFO, "socket create success, sockfd: %d\n", _sockfd);

        // 2.0 填充sockaddr_in结构
        struct sockaddr_in local; // struct sockaddr_in 系统提供的数据类型。local是变量，用户栈上开辟空间。int a = 100; a = 20;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port); // port要经过网络传输给对面，先到网络，_port：主机序列-> 主机序列，转成网络序列
        // a. 字符串风格的点分十进制的IP地址转成 4 字节IP
        // b. 主机序列，转成网络序列
        // in_addr_t inet_addr(const char *cp) -> 同时完成 a & b
        // local.sin_addr.s_addr = inet_addr(_ip.c_str()); // "192.168.3.1" -> 字符串风格的点分十进制的IP地址 -> 4字节IP
        local.sin_addr.s_addr = INADDR_ANY; // htonl(INADDR_ANY);

        // 2.1 bind sockfd和网络信息(IP(?) + Port)
        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (n < 0)
        {
            LOG(FATAL, "bind error, %s, %d\n", strerror(errno), errno);
            exit(BIND_ERROR);
        }
        LOG(INFO, "socket bind success\n");
    }
    void Start() // 所有的服务器，本质解决的是输入输出的问题！不想让网络通信模块和业务模块进行强耦合
    {
        // 一直运行，直到管理者不想运行了， 服务器都是死循环
        // UDP是面向数据报的协议
        _isrunning = true;
        while (true)
        {
            char request[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer); // 必须初始化成为sizeof(peer)
            // 1. 我们要让server先收数据
            ssize_t n = recvfrom(_sockfd, request, sizeof(request) - 1, 0, (struct sockaddr *)&peer, &len);
            if (n > 0)
            {
                request[n] = 0;
                InetAddr addr(peer);
                LOG(DEBUG, "get message from [%s:%d]: %s\n", addr.Ip().c_str(), addr.Port(), request);
                bool ok;
                std::string response = _func(request, ok); // 将请求，回调出去，在外部进行数据处理
                // (void)ok;

                // 2. 我们要将server收到的数据，发回给对方
                sendto(_sockfd, response.c_str(), response.size(), 0, (struct sockaddr *)&peer, len);
            }
        }
        _isrunning = false;
    }
    ~UdpServer()
    {
    }

private:
    int _sockfd;
    // std::string _ip; // 暂时这样写，这个地方不是必须的.TODO
    uint16_t _port;  // 服务器所用的端口号
    bool _isrunning;

    // 给服务器设定回调，用来让上层进行注册业务的处理方法
    func_t _func;
};
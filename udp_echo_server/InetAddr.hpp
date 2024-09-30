#pragma once

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class InetAddr
{
private:
    void GetAddress(std::string *ip, uint16_t *port)
    {
        *port = ntohs(_addr.sin_port);
        *ip = inet_ntoa(_addr.sin_addr);
    }

public:
    InetAddr(const struct sockaddr_in &addr) : _addr(addr)
    {
        GetAddress(&_ip, &_port);
    }
    std::string Ip()
    {
        return _ip;
    }
    uint16_t Port()
    {
        return _port;
    }
    ~InetAddr()
    {
    }

private:
    struct sockaddr_in _addr;
    std::string _ip;
    uint16_t _port;
};
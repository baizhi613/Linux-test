#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <functional>
#include "InetAddr.hpp"
#include "ThreadPool.hpp"
#include "LockGuard.hpp"

using task_t = std::function<void()>;

class MessageRoute
{
private:
    bool IsExists(const InetAddr &addr)
    {
        for (auto a : _online_user)
        {
            if (a == addr)
                return true;
        }
        return false;
    }

public:
    MessageRoute()
    {
        pthread_mutex_init(&_mutex, nullptr);
    }
    void AddUser(const InetAddr &addr)
    {
        LockGuard lockguard(&_mutex);
        if (IsExists(addr))
            return;
        _online_user.push_back(addr);
    }
    void DelUser(const InetAddr &addr)
    {
        LockGuard lockguard(&_mutex);
        for (auto iter = _online_user.begin(); iter != _online_user.end(); iter++)
        {
            if (*iter == addr)
            {
                _online_user.erase(iter);
                break;
            }
        }
    }
    void RouteHelper(int sockfd, std::string message, InetAddr who)
    {
        LockGuard lockguard(&_mutex);
        // 2. 进行消息转发
        for (auto u : _online_user)
        {
            std::string send_message = "\n[" + who.Ip() + ":" + std::to_string(who.Port()) + "]# " + message + "\n";

            struct sockaddr_in clientaddr = u.Addr();
            ::sendto(sockfd, send_message.c_str(), send_message.size(), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));
        }
    }
    void Route(int sockfd, std::string message, InetAddr who)
    {
        // 1. 我们任务，用户首次发消息，不光光是发消息，还要将自己，插入到在线用户中！
        AddUser(who);

        // 1.1 如果客户端要退出
        if (message == "Q" || message == "QUIT")
        {
            DelUser(who);
        }

        // 2. 构建任务对象，入队列，让线程池进行转发
        task_t t = std::bind(&MessageRoute::RouteHelper, this, sockfd, message, who);
        ThreadPool<task_t>::GetInstance()->Enqueue(t);
    }
    ~MessageRoute()
    {
        pthread_mutex_destroy(&_mutex);
    }

private:
    std::vector<InetAddr> _online_user;
    pthread_mutex_t _mutex;
};
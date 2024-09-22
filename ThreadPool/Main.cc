#include "ThreadPool.hpp"
#include "Task.hpp"
#include "Log.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

int main()
{
    // 获取单例的可能是多线程啊！！！
    // 程序已经加载
    LOG(DEBUG, "程序已经加载");
    sleep(3);
    ThreadPool<Task>::GetInstance();
    sleep(2);

    ThreadPool<Task>::GetInstance();
    sleep(2);

    ThreadPool<Task>::GetInstance();
    sleep(2);

    ThreadPool<Task>::GetInstance();
    sleep(2);

    ThreadPool<Task>::GetInstance()->Wait();
    sleep(2);

    return 0;
}

// class A
// {
// public:
//     void fun()
//     {
//         printf("hello world\n");
//     }
// };

// class B : public A
// {
// public:
//     void test()
//     {
//         static int count = 0;
//         A::fun();
//         count++;
//         printf("fun: %d\n", count);
//     }
// };

// int main()
// {
//     // B *obj = new B();
//     // while (true)
//     // {
//     //     obj->test();
//     //     sleep(1);
//     // }

//     srand(time(nullptr) ^ getpid() ^ pthread_self());
//     EnableScreen(); // 开启日志显示器打印功能
//     // EnableFile();
//     std::unique_ptr<ThreadPool<Task>> tp = std::make_unique<ThreadPool<Task>>(5); // C++14新特性
//     tp->InitThreadPool();
//     tp->Start();

//     int tasknum = 10;
//     while(tasknum)
//     {
//         int a = rand() % 10 + 1;
//         usleep(1234);
//         int b = rand() % 5 + 1;
//         Task t(a, b);
//         LOG(INFO, "main thread push task: %s", t.DebugToString().c_str());
//         tp->Enqueue(t);
//         sleep(1);
//         tasknum--;
//     }

//     tp->Stop();
//     tp->Wait();

//     // EnableFile();
//     // Test(7, 11,22,33, 44,55,66,77);
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld");

//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld: %s", "world");
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld: %s, %d", "world", 10);
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld: %s, %d, %f", "world", 10, 3.14);
//     // LOG(DEBUG, "hello %d, %s, %f, %d", 10, "world", 3.14, 10); // LogMessage(__FILE__, __LINE__, level, format, ...)
//     // LOG(INFO, "hello world, %d", 10);
//     // LOG(INFO, "hello world");
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld");
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld");
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld");
//     // LogMessage(__FILE__, __LINE__, DEBUG, "helloworld");
//     return 0;
// }
#include <iostream>
#include <vector>
#include "Thread.hpp"

using namespace ThreadModule;

// 数据不一致
int g_tickets = 10000; // 共享资源，没有保护的

void route(int &tickets)
{
    while (true)
    {
        if(tickets>0)
        {
            usleep(1000);
            printf("get tickets: %d\n", tickets);
            tickets--;
        }
        else
        {
            break;
        }
    }
}

const int num = 4;
int main()
{
    // std::cout << "main： &tickets: " << &g_tickets << std::endl;

    std::vector<Thread<int>> threads;
    // 1. 创建一批线程
    for (int i = 0; i < num; i++)
    {
        std::string name = "thread-" + std::to_string(i + 1);
        threads.emplace_back(route, g_tickets, name);
    }

    // 2. 启动 一批线程
    for (auto &thread : threads)
    {
        thread.Start();
    }

    // 3. 等待一批线程
    for (auto &thread : threads)
    {
        thread.Join();
        std::cout << "wait thread done, thread is: " << thread.name() << std::endl;
    }

    return 0;
}



// using namespace std;

// void threadrun(int num)
// {
//     while(num)
//     {
//         cout<<"I am a thread,num:"<<num<<endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     thread t1(threadrun,10);

//     while(true)
//     {
//         cout<<"I am a main thread"<<endl;
//         sleep(1);
//     }

//     t1.join();

//     return 0;
// }

// #include<iostream>
// #include<string>
// #include<vector>
// #include<cstdio>
// #include<cstdlib>
// #include<pthread.h>
// #include<unistd.h>
// using namespace std;

// const int threadnum = 5; // 定义线程数量为5

// // Task类，表示一个任务
// class Task {
// public:
//     Task() {} // 构造函数
//     void SetData(int x, int y) { // 设置任务数据
//         datax = x;
//         datay = y;
//     }
//     int Excute() { // 执行任务，返回数据x和数据y的和
//         return datax + datay;
//     }
//     ~Task() {} // 析构函数
// private:
//     int datax; // 任务数据x
//     int datay; // 任务数据y
// };

// // ThreadData类，继承自Task类，用于包装任务和线程名称
// class ThreadData : public Task {
// public:
//     ThreadData(int x, int y, const string &threadname)
//         : _threadname(threadname) { // 构造函数，设置线程名称
//         _t.SetData(x, y); // 设置任务数据
//     }
//     string threadname() { // 返回线程名称
//         return _threadname;
//     }
//     int run() { // 执行任务
//         return _t.Excute();
//     }
// private:
//     string _threadname; // 线程名称
//     Task _t; // 任务数据
// };

// // Result类，用于存储线程执行结果
// class Result {
// public:
//     Result() {} // 构造函数
//     ~Result() {} // 析构函数
//     void SetResult(int result, const string &threadname) { // 设置结果和线程名称
//         _result = result;
//         _threadname = threadname;
//     }
//     void Print() { // 打印结果
//         cout << _threadname << ":" << _result << endl;
//     }
// private:
//     int _result; // 结果
//     string _threadname; // 线程名称
// };


// void *handlerTask(void *args) {
//     ThreadData *td = static_cast<ThreadData*>(args); // 获取线程数据
//     string name = td->threadname(); // 获取线程名称
//     Result* res = new Result(); // 创建结果对象
//     int result = td->run(); // 执行任务
//     res->SetResult(result, name); // 设置结果和线程名称
//     cout << name << "run result:" << result << endl; // 打印结果
//     delete td; // 删除线程数据
//     sleep(2); // 线程休眠2秒
//     return res; // 返回结果对象
// }

// int main() {
//     vector<pthread_t> threads; // 线程ID向量
//     for (int i = 0; i < threadnum; i++) { // 创建线程
//         char threadname[64]; // 线程名称缓冲区
//         snprintf(threadname, 64, "Thread-%d", i + 1); // 格式化线程名称
//         ThreadData *td = new ThreadData(10, 20, threadname); // 创建线程数据
//         pthread_t tid; // 线程ID
//         pthread_create(&tid, nullptr, handlerTask, td); // 创建线程
//         threads.push_back(tid); // 添加线程ID到向量
//     }

//     vector<Result*> result_set; // 结果集合
//     void *ret = nullptr;
//     for (auto& tid : threads) { // 等待每个线程完成
//         pthread_join(tid, &ret); // 等待线程完成
//         Result* res = static_cast<Result*>(ret); // 获取返回的结果对象
//         result_set.push_back(res); // 添加结果对象到集合
//     }

//     for (auto& res : result_set) { // 打印每个线程的结果
//         res->Print(); // 打印结果
//         delete res; // 删除结果对象
//     }
// }


//同一个进程内的线程，大部分资源都是共享的，地址空间是共享的
// std::string ToHex(pthread_t tid)
// {
//     char id[64];
//     snprintf(id,sizeof(id),"0x%lx",tid);
//     return id;
// }

// using namespace std; 

// void *threadrun(void *args)
// {
//     string threadname=(char*)args;
//     int cnt=5; 
//     while(cnt)
//     {
//         cout<<threadname<<"is running:"<<cnt<<",pid:"
//         <<getpid()<<"mythread id:"
//         <<ToHex(pthread_self())<<endl;
//         sleep(1);
//         cnt--;
//     }
//     return (void*)123;
// }
// //主线程退出==进程退出==所以线程都要退出
// //1.往往我们需要main thread最后结束
// //2.线程也要被"wait",不然可能内存泄漏
// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid,nullptr,threadrun,(void*)"thread-1");

//     void* ret=nullptr;
//     int n=pthread_join(tid,&ret);
//     cout<<"main thread quit,n="<<n<<"main thread get a ret:"<<(int)ret<<endl;
//     sleep(5);
//     // int cnt=10;
//     // while(cnt)
//     // {
//     //     cout<<"main thread is running:"<<cnt<<",pid:"
//     //     <<getpid()<<"new thread id:"<<ToHex(tid)<<" "
//     //     <<"main thread id:"<<ToHex(pthread_self())<<endl;
//     //     sleep(1);
//     //     cnt--;
//     // }
//     return 0;
// }

// void *newthreadrun(void *args)
// {
//     while (true)
//     {
//         cout<<"I am new thread,pid:"<<getpid()<<endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid,nullptr,newthreadrun,nullptr);

//     while (true)
//     {
//         cout<<"I am main thread,pid:"<<getpid()<<endl;
//         sleep(1);
//     }
// }
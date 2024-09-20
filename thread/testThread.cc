#include <iostream>
#include <vector>
#include <mutex>
// #include "LockGuard.hpp"
#include "Thread.hpp"

using namespace ThreadModule;

class ThreadData
{
public:
    ThreadData(int &tickets, const std::string &name, std::mutex &mutex)
        : _tickets(tickets), _name(name), _total(0), _mutex(mutex)
    {
    }
    ~ThreadData()
    {
    }

public:
    int &_tickets; // 所有的线程，最后都会引用同一个全局的g_tickets
    std::string _name;
    int _total;
    std::mutex &_mutex;
};

// pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;

// 数据不一致
int g_tickets = 10000; // 共享资源，没有保护的, 临界资源

// void route(ThreadData *td)
// {
//     // 加锁
//     while (true)
//     {
//         // 访问临界资源的代码，叫做临界区！--- 我们加锁，本质就是把并行执行 --- 串行执行 --- 加锁的粒度要约细越好
//         // pthread_mutex_lock(&gmutex); // 加锁 : 竞争锁是自由竞争的，竞争锁的能力太强的线程，会导致其他线程抢不到锁 --- 造成了其他线程的饥饿问题！
//         pthread_mutex_lock(&td->_mutex);
//         if (td->_tickets > 0)        // 1
//         {
//             // 模拟一次抢票的逻辑
//             usleep(1000);
//             printf("%s running, get tickets: %d\n", td->_name.c_str(), td->_tickets); // 2
//             td->_tickets--;                                                           // 3
//             pthread_mutex_unlock(&td->_mutex); // 解锁
//             td->_total++;
//         }
//         else
//         {
//             pthread_mutex_unlock(&td->_mutex); // 解锁
//             break;
//         }
//     }
//     // 解锁
// }

void route(ThreadData *td)
{
    while (true)
    {
        { // 担心就用这个
            //LockGuard guard(&td->_mutex); // 临时对象， RAII风格的加锁和解锁
            // td->_mutex.lock();
            // std::lock_guard<std::mutex> lock(td->_mutex);
            if (td->_tickets > 0) // 1
            {
                usleep(1000);
                printf("%s running, get tickets: %d\n", td->_name.c_str(), td->_tickets); // 2
                td->_tickets--;                                                           // 3
                td->_total++;
                // td->_mutex.unlock();
            }
            else
            {
                // td->_mutex.unlock();
                break;
            }
        }
    }
}

const int num = 4;
int main()
{
    // std::cout << "main： &tickets: " << &g_tickets << std::endl;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);
    // std::mutex mutex;

    std::vector<Thread<ThreadData *>> threads;
    std::vector<ThreadData *> datas;
    // 1. 创建一批线程
    for (int i = 0; i < num; i++)
    {
        std::string name = "thread-" + std::to_string(i + 1);
        //ThreadData *td = new ThreadData(g_tickets, name, mutex);
        //threads.emplace_back(route, td, name);
        //datas.emplace_back(td);
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
        // std::cout << "wait thread done, thread is: " << thread.name() << std::endl;
    }
    sleep(1);
    // 4. 输出统计数据
    for (auto data : datas)
    {
        std::cout << data->_name << " : " << data->_total << std::endl;
        delete data;
    }

    // pthread_mutex_destroy(&mutex);
    return 0;
}

// using namespace ThreadModule;

// void print(int &cnt)
// {
//     while (cnt)
//     {
//         std::cout << "hello I am myself thread, cnt: " << cnt-- << std::endl;
//         sleep(1);
//     }
// }

// const int num = 10;

// int main()
// {
//     std::vector<Thread<int> > threads;
//     // 1. 创建一批线程
//     for (int i = 0; i < num; i++)
//     {
//         std::string name = "thread-" + std::to_string(i + 1);
//         threads.emplace_back(print, 10, name);
//     }

//     // 2. 启动 一批线程
//     for (auto &thread : threads)
//     {
//         thread.Start();
//     }

//     // 3. 等待一批线程
//     for (auto &thread : threads)
//     {
//         thread.Join();
//         std::cout << "wait thread done, thread is: " << thread.name() << std::endl;
//     }

//     // Thread<int> t1(print, 10);
//     // t1.Start();
//     // std::cout << "name: " << t1.name() << std::endl;
//     // t1.Join();
//     return 0;
// }

// // std::string ToHex(pthread_t tid)
// // {
// //     char id[64];
// //     snprintf(id, sizeof(id), "0x%lx", tid);
// //     return id;
// // }

// // __thread uint64_t starttime = 100;
// // // __thread int tid = 0;
// int g_val = 100;

// // 线程是可以分离的: 默认线程是joinable的。
// // 如果我们main thread不关心新线程的执行信息，我们可以将新线程设置为分离状态:
// // 你是如何理解线程分离的呢？底层依旧属于同一个进程！只是不需要等待了
// // 一般都希望mainthread 是最后一个退出的，无论是否是join、detach
// void *threadrun1(void *args)
// {
//     starttime = time(nullptr);
//     // pthread_detach(pthread_self());
//     std::string name = static_cast<const char *>(args);

//     while(true)
//     {
//         sleep(1);
//         printf("%s, g_val: %lu, &g_val: %p\n", name.c_str(), starttime, &starttime);
//     }

//     return nullptr;
// }

// void *threadrun2(void *args)
// {
//     sleep(5);
//     starttime = time(nullptr);

//     // pthread_detach(pthread_self());
//     std::string name = static_cast<const char *>(args);

//     while(true)
//     {
//         printf("%s, g_val: %lu, &g_val: %p\n", name.c_str(), starttime, &starttime);
//         sleep(1);
//     }

//     return nullptr;
// }

// int main()
// {
//     pthread_t tid1;
//     pthread_t tid2;
//     pthread_create(&tid1, nullptr, threadrun1, (void *)"thread 1");
//     pthread_create(&tid2, nullptr, threadrun2, (void *)"thread 2");

//     pthread_join(tid1, nullptr);
//     pthread_join(tid2, nullptr);
//     // pthread_detach(tid);

//     // std::cout << "new tid: " << tid << ", hex tid: " << ToHex(tid) << std::endl;
//     // std::cout << "main tid: " << pthread_self() << ", hex tid: " << ToHex(pthread_self()) << std::endl;

//     // int cnt = 5;
//     // while (true)
//     // {
//     //     if (!(cnt--))
//     //         break;
//     //     std::cout << "I am a main thread ..." << getpid() << std::endl;
//     //     sleep(1);
//     // }

//     // std::cout << "main thread wait block" << std::endl;
//     // int n = pthread_join(tid, nullptr);
//     // std::cout << "main thread wait return: " << n << ": " << strerror(n) << std::endl;

//     return 0;
// }

// #include <thread> // C++
// #include <unistd.h>

// void threadrun(int num)
// {
//     while(num)
//     {
//         std::cout << "I am a thread, num: " << num << std::endl;
//         sleep(1);
//     }
// }
// int main()
// {
//     std::thread t1(threadrun, 10);
//     std::thread t2(threadrun, 10);
//     std::thread t3(threadrun, 10);
//     std::thread t4(threadrun, 10);
//     std::thread t5(threadrun, 10);

//     while(true)
//     {
//         std::cout << "I am a main thread "<< std::endl;
//         sleep(1);
//     }

//     t1.join();
//     t2.join();
//     t3.join();
//     t4.join();
// t5.join();

//     return 0;
// }

// #include <iostream>
// #include <string>
// #include <vector>
// #include <cstdio>
// #include <unistd.h>
// #include <cstdlib>
// #include <pthread.h> // 原生线程库的头文件

// const int threadnum = 5;

// class Task
// {
// public:
//     Task()
//     {}
//     void SetData(int x, int y)
//     {
//         datax = x;
//         datay = y;
//     }
//     int Excute()
//     {
//         return datax + datay;
//     }
//     ~Task()
//     {

//     }
// private:
//     int datax;
//     int datay;
// };

// class ThreadData : public Task
// {
// public:
//     ThreadData(int x, int y, const std::string &threadname):_threadname(threadname)
//     {
//         _t.SetData(x, y);
//     }
//     std::string threadname()
//     {
//         return _threadname;
//     }
//     int run()
//     {
//         return _t.Excute();
//     }
// private:
//     std::string _threadname;
//     Task _t;
// };

// class Result
// {
// public:
//     Result(){}
//     ~Result(){}
//     void SetResult(int result, const std::string &threadname)
//     {
//         _result = result;
//         _threadname = threadname;
//     }
//     void Print()
//     {
//         std::cout << _threadname << " : " << _result << std::endl;
//     }
// private:
//     int _result;
//     std::string _threadname;
// };

// void *handlerTask(void *args)
// {
//     ThreadData *td = static_cast<ThreadData *>(args);

//     std::string name = td->threadname();

//     Result *res = new Result();
//     int result = td->run();

//     res->SetResult(result, name);

//     // std::cout << name << "run result : " << result << std::endl;
//     delete td;

//     sleep(2);
//     return res;
//     // // std::string threadname =static_cast<char*>(args);
//     // const char *threadname = static_cast<char *>(args);
//     // while (true)
//     // {
//     //     std::cout << "I am " << threadname << std::endl;
//     //     sleep(2);
//     // }

//     // delete []threadname;

//     // return nullptr;
// }
// // 1. 多线程创建
// // 2. 线程传参和返回值，我们可以传递级别信息，也可以传递其他对象(包括你自己定义的！)
// // 3. C++11也带了多线程，和我们今天的是什么关系？？？ TODO下节课
// int main()
// {
//     std::vector<pthread_t> threads;
//     for (int i = 0; i < threadnum; i++)
//     {
//         char threadname[64];
//         snprintf(threadname, 64, "Thread-%d", i + 1);
//         ThreadData *td = new ThreadData(10, 20, threadname);

//         pthread_t tid;
//         pthread_create(&tid, nullptr, handlerTask, td);
//         threads.push_back(tid);
//     }
//     std::vector<Result*> result_set;
//     void *ret = nullptr;
//     for (auto &tid : threads)
//     {
//         pthread_join(tid, &ret);
//         result_set.push_back((Result*)ret);
//     }

//     for(auto & res : result_set)
//     {
//         res->Print();
//         delete res;
//     }
// }

// // 同一个进程内的线程，大部分资源都是共享的. 地址空间是共享的！
// int g_val = 100;

// std::string ToHex(pthread_t tid)
// {
//     char id[64];
//     snprintf(id, sizeof(id), "0x%lx", tid);
//     return id;
// }

// // 线程退出
// // 1. 代码跑完，结果对
// // 2. 代码跑完，结果不对
// // 3. 出异常了 --- 重点 --- 多线程中，任何一个线程出现异常(div 0， 野指针), 都会导致整个进程退出！ ---- 多线程代码往往健壮性不好
// void *threadrun(void *args)
// {
//     std::string threadname = (char*)args;
//     int cnt = 5;
//     while (cnt)
//     {
//         printf("new thread, g_val: %d, &g_val: %p\n", g_val, &g_val);

//         // std::cout << threadname << " is running: " << cnt << ", pid: " << getpid()
//         //     << " mythread id: " << ToHex(pthread_self())
//         //     << "g_val: "<< g_val << " &g_val: " << &g_val << std::endl;
//         g_val++;
//         sleep(1);
//         // int *p = nullptr;
//         // *p = 100; // 故意一个野指针
//         cnt--;
//     }
//     // 1. 线程函数结束
//     // 2. pthread_exit()
//     // pthread_exit((void*)123);
//     // exit(10); // 不能用exit终止线程，因为它是终止进程的.
//     return (void*)123; // warning
// }

// // 主线程退出 == 进程退出 == 所有线程都要退出
// // 1. 往往我们需要main thread最后结束
// // 2. 线程也要被"wait", 要不然会产生类似进程哪里的内存泄漏的问题
// int main()
// {
//     // 1. id
//     pthread_t tid;
//     pthread_create(&tid, nullptr, threadrun, (void*)"thread-1");

//     // 在主线程中，你保证新线程已经启动
//     // sleep(2);
//     // pthread_cancel(tid);

//     void* ret = nullptr;

//     // 2. 新和主两个线程，谁先运行呢？不确定，由调度器决定
//     // int cnt = 10;
//     // while (true)
//     // {
//     //     // std::cout << "main thread is running: " << cnt << ", pid: "
//     //     //     << getpid() << " new thread id: " << ToHex(tid) << " "
//     //     //     << " main thread id: " << ToHex(pthread_self())
//     //     //     << "g_val: "<< g_val << " &g_val: " << &g_val << std::endl;
//     //     printf("main thread, g_val: %d, &g_val: %p\n", g_val, &g_val);

//     //     sleep(1);
//     //     cnt--;
//     // }
//     // 不考虑线程异常情况！
//     // PTHREAD_CANCELED; // (void*)-1
//     int n = pthread_join(tid, &ret); // 我们怎么没有像进程一样获取线程退出的退出信号呢？只有你手动写的退出码
//     std::cout << "main thread quit, n=" << n << " main thread get a ret: " << (long long)ret << std::endl;
//     return 0;
// }

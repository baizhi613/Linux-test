#include "RingQueue.hpp"
#include "Thread.hpp"
#include "Task.hpp"
#include <string>
#include <vector>
#include <unistd.h>
#include <ctime>

// 我们需要的是向队列中投递任务
using namespace ThreadModule;
using ringqueue_t = RingQueue<Task>;

void Consumer(ringqueue_t &rq, std::string name)
{
    while (true)
    {
        sleep(2);
        // 1. 消费任务
        Task t;
        rq.Pop(&t);
        std::cout << "Consumer handler task: " << "[" << name << "]" << std::endl;
        // 2. 处理任务
        t();
    }
}

void Productor(ringqueue_t &rq, std::string name)
{
    srand(time(nullptr) ^ pthread_self());
    //int cnt = 10;
    while (true)
    {
        // 获取任务
        // 生产任务
        rq.Enqueue(Download);
        std::cout << "Productor : " << "[" << name << "]" << std::endl;
        // cnt--;
    }
}

void InitComm(std::vector<Thread<ringqueue_t>> *threads, int num, ringqueue_t &rq, func_t<ringqueue_t> func, const std::string &who)
{
    for (int i = 0; i < num; i++)
    {
        std::string name = "thread-" + std::to_string(i + 1) + "-" + who;
        threads->emplace_back(func, rq, name);
        // threads->back()->Start();
    }
}

void InitConsumer(std::vector<Thread<ringqueue_t>> *threads, int num, ringqueue_t &rq)
{
    InitComm(threads, num, rq, Consumer, "consumer");
}

void InitProductor(std::vector<Thread<ringqueue_t>> *threads, int num, ringqueue_t &rq)
{
    InitComm(threads, num, rq, Productor, "productor");
}

void WaitAllThread(std::vector<Thread<ringqueue_t>> &threads)
{
    for (auto &thread : threads)
    {
        thread.Join();
    }
}

    
void StartAll(std::vector<Thread<ringqueue_t>> &threads)
{
    for(auto &thread : threads)
    {
        std::cout << "start: " << thread.name() << std::endl;
        thread.Start();
    }
}

int main()
{
    ringqueue_t *rq = new ringqueue_t(10);
    std::vector<Thread<ringqueue_t>> threads;
    // std::vector<Thread<ThreadData>> threads;

    InitProductor(&threads, 1, *rq);
    InitConsumer(&threads, 1, *rq);

    StartAll(threads);

    WaitAllThread(threads);

    return 0;
}
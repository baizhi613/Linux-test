#include "BlockQueue.hpp"
#include "Thread.hpp"
#include <string>
#include <vector>
#include <unistd.h>

using namespace ThreadModule;
int a = 10;

void Consumer(BlockQueue<int> &bq)
{
    // sleep(10);
    while (true)
    {
        int data;
        bq.Pop(&data);
        std::cout << "Consumer Consum data is : " << data << " addr: " << &bq << std::endl;
        sleep(2);
    }
}

void Productor(BlockQueue<int> &bq)
{
    int cnt = 1;
    while (true)
    {
        bq.Enqueue(cnt);
        std::cout << "Productor product data is : " << cnt << " addr: " << &bq << std::endl;
        cnt++;
    }
}

void StartComm(std::vector<Thread<BlockQueue<int>>> *threads, int num, BlockQueue<int> &bq, func_t<BlockQueue<int>> func)
{
    for (int i = 0; i < num; i++)
    {
        std::string name = "thread-" + std::to_string(i + 1);
        threads->emplace_back(func, bq, name);
        threads->back().Start();
    }
}

void StartConsumer(std::vector<Thread<BlockQueue<int>>> *threads, int num, BlockQueue<int> &bq)
{
    StartComm(threads, num, bq, Consumer);
}

void StartProductor(std::vector<Thread<BlockQueue<int>>> *threads, int num, BlockQueue<int> &bq)
{
    StartComm(threads, num, bq, Productor);
}

void WaitAllThread(std::vector<Thread<BlockQueue<int>>> &threads)
{
    for (auto &thread : threads)
    {
        thread.Join();
    }
}

int main()
{
    BlockQueue<int> *bq = new BlockQueue<int>(5);
    std::vector<Thread<BlockQueue<int>>> threads;

    StartProductor(&threads, 1, *bq);
    StartConsumer(&threads, 1, *bq);
    WaitAllThread(threads);

    return 0;
}
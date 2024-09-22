#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <pthread.h>
#include "Log.hpp"
#include "Thread.hpp"
#include "LockGuard.hpp"

using namespace ThreadModule;

const static int gdefaultthreadnum = 10;

// 日志
template <typename T>
class ThreadPool
{
private:
    void LockQueue()
    {
        pthread_mutex_lock(&_mutex);
    }
    void UnlockQueue()
    {
        pthread_mutex_unlock(&_mutex);
    }
    void ThreadSleep()
    {
        pthread_cond_wait(&_cond, &_mutex);
    }
    void ThreadWakeup()
    {
        pthread_cond_signal(&_cond);
    }
    void ThreadWakeupAll()
    {
        pthread_cond_broadcast(&_cond);
    }
    // 是要有的，必须是私有的
    ThreadPool(int threadnum = gdefaultthreadnum) : _threadnum(threadnum), _waitnum(0), _isrunning(false)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);
        LOG(INFO, "ThreadPool Construct()");
    }
    void InitThreadPool()
    {
        // 指向构建出所有的线程，并不启动
        for (int num = 0; num < _threadnum; num++)
        {
            std::string name = "thread-" + std::to_string(num + 1);
            _threads.emplace_back(std::bind(&ThreadPool::HandlerTask, this, std::placeholders::_1), name);
            LOG(INFO, "init thread %s done", name.c_str());
        }
        _isrunning = true;
    }
    void Start()
    {
        for (auto &thread : _threads)
        {
            thread.Start();
        }
    }
    void HandlerTask(std::string name) // 类的成员方法，也可以成为另一个类的回调方法，方便我们继续类级别的互相调用！
    {
        LOG(INFO, "%s is running...", name.c_str());
        while (true)
        {
            // 1. 保证队列安全
            LockQueue();
            // 2. 队列中不一定有数据
            while (_task_queue.empty() && _isrunning)
            {
                _waitnum++;
                ThreadSleep();
                _waitnum--;
            }
            // 2.1 如果线程池已经退出了 && 任务队列是空的
            if (_task_queue.empty() && !_isrunning)
            {
                UnlockQueue();
                break;
            }
            // 2.2 如果线程池不退出 && 任务队列不是空的
            // 2.3 如果线程池已经退出 && 任务队列不是空的 --- 处理完所有的任务，然后在退出
            // 3. 一定有任务, 处理任务
            T t = _task_queue.front();
            _task_queue.pop();
            UnlockQueue();
            LOG(DEBUG, "%s get a task", name.c_str());
            // 4. 处理任务，这个任务属于线程独占的任务
            t();
            LOG(DEBUG, "%s handler a task, result is: %s", name.c_str(), t.ResultToString().c_str());
        }
    }
    // 复制拷贝禁用
    ThreadPool<T> &operator=(const ThreadPool<T> &) = delete;
    ThreadPool(const ThreadPool<T> &) = delete;

public:
    static ThreadPool<T> *GetInstance()
    {
        // 如果是多线程获取线程池对象下面的代码就有问题了！！
        // 只有第一次会创建对象，后续都是获取
        // 双判断的方式，可以有效减少获取单例的加锁成本，而且保证线程安全
        if (nullptr == _instance) // 保证第二次之后，所有线程，不用在加锁，直接返回_instance单例对象
        {
            LockGuard lockguard(&_lock);
            if (nullptr == _instance)
            {
                _instance = new ThreadPool<T>();
                _instance->InitThreadPool();
                _instance->Start();
                LOG(DEBUG, "创建线程池单例");
                return _instance;
            }
        }
        LOG(DEBUG, "获取线程池单例");
        return _instance;
    }

    void Stop()
    {
        LockQueue();
        _isrunning = false;
        ThreadWakeupAll();
        UnlockQueue();
    }
    void Wait()
    {
        for (auto &thread : _threads)
        {
            thread.Join();
            LOG(INFO, "%s is quit...", thread.name().c_str());
        }
    }
    bool Enqueue(const T &t)
    {
        bool ret = false;
        LockQueue();
        if (_isrunning)
        {
            _task_queue.push(t);
            if (_waitnum > 0)
            {
                ThreadWakeup();
            }
            LOG(DEBUG, "enqueue task success");
            ret = true;
        }
        UnlockQueue();
        return ret;
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);
    }

private:
    int _threadnum;
    std::vector<Thread> _threads; // for fix, int temp
    std::queue<T> _task_queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    int _waitnum;
    bool _isrunning;

    // 添加单例模式
    static ThreadPool<T> *_instance;
    static pthread_mutex_t _lock;
};

template <typename T>
ThreadPool<T> *ThreadPool<T>::_instance = nullptr;

template <typename T>
pthread_mutex_t ThreadPool<T>::_lock = PTHREAD_MUTEX_INITIALIZER;

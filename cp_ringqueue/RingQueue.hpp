#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <semaphore.h>
#include <pthread.h>

// 单生产，单消费
// 多生产，多消费
// "321":
// 3: 三种关系
// a: 生产和消费互斥和同步
// b: 生产者之间:
// c: 消费者之间:
// 解决方案：加锁
// 1. 需要几把锁？2把
// 2. 如何加锁？
template<typename T>
class RingQueue
{
private:
    void P(sem_t &sem)
    {
        sem_wait(&sem);
    }
    void V(sem_t &sem)
    {
        sem_post(&sem);
    }
    void Lock(pthread_mutex_t &mutex)
    {
        pthread_mutex_lock(&mutex);
    }
    void Unlock(pthread_mutex_t &mutex)
    {
        pthread_mutex_unlock(&mutex);
    }
public:
    RingQueue(int cap): _ring_queue(cap), _cap(cap),  _productor_step(0), _consumer_step(0)
    {
        sem_init(&_room_sem, 0, _cap);
        sem_init(&_data_sem, 0, 0);

        pthread_mutex_init(&_productor_mutex, nullptr);
        pthread_mutex_init(&_consumer_mutex, nullptr);
    }
    void Enqueue(const T &in)
    {
        // 生产行为
        P(_room_sem);
        Lock(_productor_mutex);
        // 一定有空间！！！
        _ring_queue[_productor_step++] = in; // 生产
        _productor_step %= _cap;
        Unlock(_productor_mutex);
        V(_data_sem);
    }
    void Pop(T *out)
    {
        // 消费行为
        P(_data_sem);
        Lock(_consumer_mutex);
        *out = _ring_queue[_consumer_step++];
        _consumer_step %= _cap;
        Unlock(_consumer_mutex);
        V(_room_sem);
    }
    ~RingQueue()
    {
        sem_destroy(&_room_sem);
        sem_destroy(&_data_sem);

        pthread_mutex_destroy(&_productor_mutex);
        pthread_mutex_destroy(&_consumer_mutex);
    }
private:
    // 1. 环形队列
    std::vector<T> _ring_queue;
    int _cap; // 环形队列的容量上限

    // 2. 生产和消费的下标
    int _productor_step;
    int _consumer_step;

    // 3. 定义信号量
    sem_t _room_sem; // 生产者关心
    sem_t _data_sem; // 消费者关心

    // 4. 定义锁，维护多生产多消费之间的互斥关系
    pthread_mutex_t _productor_mutex;
    pthread_mutex_t _consumer_mutex;
};
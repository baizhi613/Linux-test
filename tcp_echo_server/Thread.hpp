#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include <iostream>
#include <string>
#include <unistd.h>
#include <functional>
#include <pthread.h>

namespace ThreadModule
{
    using func_t = std::function<void(std::string)>;

    class Thread
    {
    public:
        void Excute()
        {
            _func(_threadname);
        }
    public:
        Thread(func_t func, std::string name="none-name")
            : _func(func), _threadname(name), _stop(true)
        {}
        static void *threadroutine(void *args) // 类成员函数，形参是有this指针的！！
        {
            Thread *self = static_cast<Thread *>(args);
            self->Excute();
            return nullptr;
        }
        bool Start()
        {
            int n = pthread_create(&_tid, nullptr, threadroutine, this);
            if(!n)
            {
                _stop = false;
                return true;
            }
            else
            {
                return false;
            }
        }
        void Detach()
        {
            if(!_stop)
            {
                pthread_detach(_tid);
            }
        }
        void Join()
        {
            if(!_stop)
            {
                pthread_join(_tid, nullptr);
            }
        }
        std::string name()
        {
            return _threadname;
        }
        void Stop()
        {
            _stop = true;
        }
        ~Thread() {}

    private:
        pthread_t _tid;
        std::string _threadname;
        func_t _func;
        bool _stop;
    };
} // namespace ThreadModule

#endif
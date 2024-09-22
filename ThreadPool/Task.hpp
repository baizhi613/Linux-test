#pragma once

#include <iostream>
#include <string>
#include <functional>

// 任务类型
// using Task = std::function<void()>;

class Task
{
public:
    Task() {}
    Task(int a, int b) : _a(a), _b(b), _result(0)
    {
    }
    void Excute()
    {
        _result = _a + _b;
    }
    std::string ResultToString()
    {
        return std::to_string(_a) + "+" + std::to_string(_b) + "=" + std::to_string(_result);
    }
    std::string DebugToString()
    {
        return std::to_string(_a) + "+" + std::to_string(_b) + "=?";
    }
    void operator()()
    {
        Excute();
    }

private:
    int _a;
    int _b;
    int _result;
};

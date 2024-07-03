#pragma once

#include<iostream>
#include<unistd.h>

using namespace std;

typedef void(*work_t)();//函数指针类型
typedef void(*task_t)();

void PrintLog()
{
    cout<<"printf log task"<<endl;
}

void ReloadConf()
{
    cout<<"reload conf task"<<endl;
}

void ConnectMysql()
{
    cout<<"connect mysql task"<<endl;
}

task_t tasks[3]={PrintLog,ReloadConf,ConnectMysql};

uint32_t NextTask()
{
    return rand()%3;
}

void worker()
{
    while(true)
    {
        uint32_t command_code=0;
        ssize_t n=read(0,&command_code,sizeof(command_code));
        if(n==sizeof(command_code))
        {
            if(command_code>=3) continue;
            tasks[command_code]();
        }
        cout<<"I am a worker:"<<getpid()<<endl;
        sleep(1);
    }
}

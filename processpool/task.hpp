#pragma once

#include<iostream>
#include<unistd.h>

using namespace std;

typedef void(*work_t)(int);//函数指针类型
typedef void(*task_t)(int,pid_t);

void PrintLog(int fd,pid_t pid)
{
    cout<<"sub process:"<<pid<<",fd:"<<fd<<",task is:printf log task\n"<<endl;
}

void ReloadConf(int fd,pid_t pid)
{
    cout<<"sub process:"<<pid<<",fd:"<<fd<<",task is:reload conf task\n"<<endl;
}

void ConnectMysql(int fd,pid_t pid)
{
    cout<<"sub process:"<<pid<<",fd:"<<fd<<",task is:connect mysql task\n"<<endl;
}

task_t tasks[3]={PrintLog,ReloadConf,ConnectMysql};

uint32_t NextTask()
{
    return rand()%3;
}

void worker(int fd)
{
    while(true)
    {
        uint32_t command_code=0;
        ssize_t n=read(0,&command_code,sizeof(command_code));
        if(n==sizeof(command_code))
        {
            if(command_code>=3) continue;
            tasks[command_code](fd,getpid());
        }
        else if(n==0)
        {
            std::cout<<"sub process:"<<getpid()<<"quit now..."<<std::endl;
            break;
        }
    }
}

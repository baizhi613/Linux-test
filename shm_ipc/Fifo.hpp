#ifndef __COMM_HPP__
#define __COMM_HPP__

#include<iostream>
#include<string>
#include<cerrno>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include <cassert>

using namespace std;

#define Mode 0666
#define Path "fifo"

class Fifo
{
public:
    Fifo(const string &path=Path):_path(path)
    {
        umask(0);
        int n=mkfifo(_path.c_str(),Mode);
        if(n==0)
        {
            cout<<"mkfifo success"<<endl;
        }
        else
        {
            cerr<<"mkfifo failed,errno:"<<errno<<",errstring:"<<strerror(errno)<<endl;
        }
    }
    ~Fifo()
    {
        int n=unlink(_path.c_str());
        if(n==0)
        {
            cout<<"remove fifo file"<<_path<<"success"<<endl;
        }
        else
        {
            cerr<<"remove failed,errno:"<<errno<<",errstring:"<<strerror(errno)<<endl;
        }
    }
private:
    string _path;
};

class Sync
{
public:
    Sync()
    {
    }
    void OpenReadOrDie()
    {
        rfd=open(Path,O_RDONLY);
        if(rfd<0)
            exit(1);
    }
    void OpenWriteOrDie ()
    {
        wfd=open(Path,O_WRONLY);
        if(wfd<0)
            exit(1);
    }
    bool Wait()
    {
        bool ret=true;
        uint32_t c=0;
        ssize_t n=read(rfd,&c,sizeof(uint32_t));
        if(n==sizeof(uint32_t))
        {
            std::cout<<"server wakeup,begin read shm..."<<std::endl;
        }
        else if(n==0)
        {
            ret=false;
        }
        else
        {
            return false;
        }
        return ret;
    }
    void Wakeup()
    {
        uint32_t c=0;
        ssize_t n=write(wfd,&c,sizeof(uint32_t));
        assert(n==sizeof(uint32_t));
        std::cout<<"wakeup server..."<<std::endl;
    }
    ~Sync(){}
private:
    int rfd=-1;
    int wfd=-1;
};

#endif
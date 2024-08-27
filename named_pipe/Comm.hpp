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

using namespace std;

#define Mode 0666
#define Path "fifo"

class Fifo
{
public:
    Fifo(const string &path):_path(path)
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

#endif
#include<iostream>
#include<string>
#include<pthread.h>
#include<vector>
#include<unistd.h>

pthread_cond_t gcond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t gmutex=PTHREAD_MUTEX_INITIALIZER;

void *SlaverCore(void *args)
{
    std::string name=static_cast<const char*>(args);
    while(true)
    {
        pthread_mutex_lock(&gmutex);
        pthread_cond_wait(&gcond,&gmutex);
        std::cout<<"当前被叫醒的线程是："<<name<<std::endl;
        pthread_mutex_unlock(&gmutex);
    }
}

void *MasterCore(void *args)
{
    sleep(3);
    std::cout<<"master开始工作..."<<std::endl;
    std::string name=static_cast<const char*>(args);
    while(true)
    {
        pthread_cond_signal(&gcond);
        std::cout<<"master唤醒一个线程..."<<std::endl;
        sleep(1);
    }
}

void StartMaster(std::vector<pthread_t> *tidsptr)
{
    pthread_t tid;
    int n=pthread_create(&tid,nullptr,MasterCore,(void*)"Master Thread");
    if(n==0)
    {
        std::cout<<"create success"<<std::endl;
    }
    tidsptr->emplace_back(tid);
}

void StartSlaver(std::vector<pthread_t> *tidsptr,int threadnum=3)
{
    for(int i=0;i<threadnum;i++)
    {
        char *name=new char[64];
        snprintf(name,64,"slaver-%d",i+1);
        pthread_t tid;
        int n=pthread_create(&tid,nullptr,SlaverCore,name);
        if(n==0)
        {
            std::cout<<"create success:"<<name<<std::endl;
            tidsptr->emplace_back(tid);
        }
    }
}

void WaitThread(std::vector<pthread_t> &tids)
{
    for(auto &tid:tids)
    {
        pthread_join(tid,nullptr);
    }
}

int main()
{
    std::vector<pthread_t> tids;
    StartMaster(&tids);
    StartSlaver(&tids,5);
    WaitThread(tids);
    return 0;
}
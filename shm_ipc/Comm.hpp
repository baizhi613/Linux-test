#pragma once

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <unistd.h>

using namespace std;

const char *pathname = "/home/lys";
const int proj_id = 0x66;
const int defaultsize = 4096;

std::string ToHex(key_t k)
{
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), "0x%x", k);
    return buffer;
}

key_t GetShmKeyOrDie()
{
    key_t k = ftok(pathname, proj_id);
    if (k < 0)
    {
        std::cerr << "ftok error,errno:" << errno << ",error string:" << strerror(errno) << std::endl;
        exit(1);
    }
    return k;
}

int CreateShmOrDie(key_t key, int size, int flag)
{
    int shmid = shmget(key, size, flag);
    if (shmid < 0)
    {
        std::cerr << "shmget error,errno:" << errno << ",error string:" << strerror(errno) << std::endl;
        exit(2);
    }
    return shmid;
}

int CreatShm(key_t key, int size)
{
    return CreateShmOrDie(key, size, IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm(key_t key, int size)
{
    return CreateShmOrDie(key, size, IPC_CREAT);
}

void DeleteShm(int shmid)
{
    int n = shmctl(shmid, IPC_RMID, nullptr);
    if (n < 0)
    {
        std::cerr << "shmctl error" << std::endl;
    }
    else
    {
        std::cout << "shmctl delete shm success,shmid:" << shmid << std::endl;
    }
}

void ShmDebug(int shmid)
{
    struct shmid_ds shmds;
    int n = shmctl(shmid, IPC_STAT, &shmds);
    if (n < 0)
    {
        std::cerr << "shmctl error" << std::endl;
        return;
    }
    std::cout << "shmds.shm_segsz:" << shmds.shm_segsz << std::endl;
    std::cout << "shmds.shm_nattch:" << shmds.shm_nattch << std::endl;
    std::cout << "shmds.shm_ctime:" << shmds.shm_ctime << std::endl;
    std::cout << "shmds.shm_perm.__key:" << shmds.shm_perm.__key << std::endl;
}

void *ShmAttach(int shmid)
{
    void *addr = shmat(shmid, nullptr, 0);
    if ((long long)addr == -1)
    {
        std::cerr << "shmat error" << std::endl;
        return nullptr;
    }
    return addr;
}

void ShmDetach(void *addr)
{
    int n=shmdt(addr);
    if(n<0)
    {
        std::cerr<<"shmdt error"<<std::endl;
    }
}
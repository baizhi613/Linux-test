#include "Comm.hpp"
#include"Fifo.hpp"
int main()
{
    Sync syn;
    syn.OpenWriteOrDie();
    
    key_t key = GetShmKeyOrDie();
    std::cout << "key:" << ToHex(key) << std::endl;
    // sleep(2);

    int shmid = GetShm(key, defaultsize);
    std::cout << "shmid:" << shmid << std::endl;
    // sleep(2);

    char *addr = (char *)ShmAttach(shmid);
    std::cout << "Attach shm success,addr:" <<ToHex((uint64_t)addr)<< std::endl;
    // sleep(2);

    memset(addr,0,defaultsize);
    for(char c='A';c<'Z';c++)
    {
        addr[c-'A']=c;
        sleep(1);
        syn.Wakeup();
    }

    ShmDetach(addr);
    std::cout << "Detach shm success,addr:" <<ToHex((uint64_t)addr)<< std::endl;

    sleep(5);
    DeleteShm(shmid);

    return 0;
}
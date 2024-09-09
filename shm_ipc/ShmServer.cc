#include "Comm.hpp"
#include"Fifo.hpp"
int main()
{
    key_t key = GetShmKeyOrDie();
    std::cout << "key:" << ToHex(key) << std::endl;
    // sleep(2);

    int shmid = CreatShm(key, defaultsize);
    std::cout << "shmid:" << shmid << std::endl;
    // sleep(2);   

    // ShmDebug(shmid);
    char *addr = (char *)ShmAttach(shmid);
    std::cout << "Attach shm success,addr:" <<ToHex((uint64_t)addr)<< std::endl;
    // sleep(2);

    Fifo fifo;
    Sync syn;
    syn.OpenReadOrDie();

    for(;;)
    {
        if(!syn.Wait()) break;
        cout<<"shm content:"<<addr<<std::endl;
        sleep(1); 
    }

    ShmDetach(addr);
    std::cout << "Detach shm success,addr:" <<ToHex((uint64_t)addr)<< std::endl;

    // sleep(10);
    DeleteShm(shmid);
    return 0;
}
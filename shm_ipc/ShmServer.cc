#include "Comm.hpp"

int main()
{
    key_t key = GetShmKeyOrDie();
    std::cout << "key:" << ToHex(key) << std::endl;
    sleep(2);

    int shmid = CreatShm(key, defaultsize);
    std::cout << "shmid:" << shmid << std::endl;

    // ShmDebug(shmid);

    char *addr = (char *)ShmAttach(shmid);
    std::cout << "Attach shm success" << std::endl;
    sleep(10);

    sleep(100);
    DeleteShm(shmid);
    return 0;
}
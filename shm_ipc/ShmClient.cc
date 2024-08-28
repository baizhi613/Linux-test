#include "Comm.hpp"

int main()
{
    key_t key = GetShmKeyOrDie();
    std::cout << "key:" << ToHex(key) << std::endl;
    sleep(2);

    int shmid = GetShm(key, defaultsize);
    std::cout << "shmid:" << shmid << std::endl;
    sleep(2);

    ShmAttach(shmid);

    sleep(5);

    return 0;
}
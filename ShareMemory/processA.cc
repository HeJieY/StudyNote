#include "shareMem.hpp"

int main()
{
    int shmid = getShareMem();
    char* shareMem = (char*)shmat(shmid,NULL,0);
    while(1)
    {
        printf("client say#:%s",shareMem);
        sleep(2);
    }
    shmdt(shareMem);
    return 0;
}
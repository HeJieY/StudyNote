#include "shareMem.hpp"

int main()
{
    int cnt = 2;
    int shareMemId = getShareMem();
    //这是shmat函数，它的作用是根据shmid将内存申请的空间挂在到当前地址的虚拟地址空间，此时这个进程可以使用这块空间了
    char* shareMem = (char*)shmat(shareMemId,NULL,0);
    //这里开始写通信命令，共享空间可以直接从返回的指针开始写内容，这个空间没有异步同步机制，TODO：可以依靠管道/锁来阻塞
    while(cnt)
    {
        printf("Please Enter@:");
        fgets(shareMem,4096,stdin); //直接从键盘读取数据到虚拟地址空间中，这种方式，效率高，没有经过多次拷贝
        printf("\n");
        cnt--;
    }
   
    //这是去关联函数，但是，此时共享空间仍在os中，没有删除
    shmdt(shareMem);
    //这是控制共享空间的函数，第二个第三个参数配合，可以拿到一些共享空间的一些数据，以下是操控删除该空间
    shmctl(shareMemId,IPC_RMID,NULL);
    return 0;
}
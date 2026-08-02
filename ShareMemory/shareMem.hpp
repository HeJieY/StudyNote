#ifndef _SHARE_MEM_
#define _SHARE_MEM_
//这是一个预处理措施，它和program once的作用一样
#include "log.hpp"
#include <sys/ipc.h>
#include <sys/shm.h>
const static char* pathName = ".";
const static int proj_id = 0x666;

Log myLog(SCREEN_LOG);
#endif
key_t getKey()
{
    key_t key = ftok(pathName,proj_id);
    //这是ftok函数其作用是靠路径和id根据系统内的算法算出操作系统中的一个唯一key值，用来给shmget创建空间
    if(key == - 1)
    {
        myLog.screenPrint(FATAL,"get key error: %s\n",strerror(errno));
        exit(-1);
    }

    return key;
}

int getShareMem()
{
    int shareMemId = shmget(getKey(),4096,IPC_CREAT|0666);
    //这是shmget函数，它的是在内存开一块共享空间，返回这个空间的shmid，注意此时只是创造了这个空间，该进程还没有获得这块空间，这块空间的生命周期是随着linux内核的
    if(shareMemId == -1)
    {
        myLog.screenPrint(FATAL,"create shareMem fail: %s",strerror(errno));
        return -1;
    }
    return shareMemId;
}

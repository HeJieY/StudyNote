#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
void* threadRun(void* args)
{
    while(1)
    {
        printf("I an new thread!\n");
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid,nullptr,threadRun,nullptr);
    while(1)
    {
        printf("I am main thread!\n");//对于第一个创建出来的线程我们叫做主线程，后来的是新线程
        sleep(1);
    }
    return 0;
}
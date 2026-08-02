#pragma once
//关于函数指针： 
//  1.void*(*pFun)(int x,int y):定义一个返回值为void*，参数为两个int的函数指针
//  2.typedef void*(*pFun)(int x,int y):定义的内容与上面是一致的，但这种写法将pFun从变量变成了一个类型
#include<pthread.h>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<vector>
#include<queue>


typedef void*(*task_t)(void*);
class Thread
{
public:
    Thread(const std::string& threadName):
        threadName_(threadName)
    {
        
    }
    ~Thread()
    {
        
    }
const std::string& getName()
{
    return threadName_;
}
const pthread_t& getTid()
{
    return tid_;
}

void run(task_t task)
{
    pthread_create(&tid_,nullptr,task,this);
}
void join()
{
    pthread_join(tid_,nullptr);
    printf("回收了线程:%s\n",threadName_.c_str());
}


private:
    pthread_t tid_;
    const std::string threadName_;
};
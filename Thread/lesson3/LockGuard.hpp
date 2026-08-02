//关于pthread_mutex_t:
//1. 该参数目前所知无法进行值传递，参数绑定了内核状态等，值拷贝会导致状态混乱，锁失效
//2. 不可以多次初始化，这种行为是不安全的，会导致锁失效
#pragma once
#include<pthread.h>
#include<cstdio>
class Mutex
{
public:
    Mutex()
    {
        init();
    }
    ~Mutex()
    {
        destory();
    }
    pthread_mutex_t* getLock()
    {
        return &_lock;
    }
private:
    pthread_mutex_t _lock;
    void init()
    {
        pthread_mutex_init(&_lock,nullptr);
       
    }
    void destory()
    {
        pthread_mutex_destroy(&_lock);
    }
};
class LockGuard
{
public:
    LockGuard(Mutex* mutex):
        _mutex(mutex)
    {
        lock();
    }
    ~LockGuard()
    {
        unlock();
    }
   
private:
    Mutex* _mutex;
    void lock()
    {
        pthread_mutex_lock(_mutex->getLock());
    }
    void unlock()
    {
        pthread_mutex_unlock(_mutex->getLock());
    }
};
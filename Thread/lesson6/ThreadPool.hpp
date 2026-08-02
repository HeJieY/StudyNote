#pragma once
#include"Thread.hpp"
#include"Task.hpp"
template<class T>
class ThreadPool
{
    const static int DEFAULT_THREAD_SIZE = 5;
    const static int DEFAULT_TASK_SIZE = 10;

    void start()
    {
        for(int i=0;i<threadNum_;i++)
        {
            Thread thread("thread"+=std::to_string(i));
            threads_.push(thread);
            thread.run()
        }

    }

    void push(T val)
    {
        wait();
        lock();
        tasks_.push(val);
    }

private:
    ThreadPool(int threadNum = DEFAULT_THREAD_SIZE,int  = DEFAULT_TASK_SIZE):
    threadNum_(threadNum),taskNum_(taskNum)
    {
        pthread_cond_init(&cond,nullptr);
        pthread_mutex_init(&mutex_,nullptr);
    }
    ~ThreadPool()
    {
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex_);
    }
    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    void wait()
    {
        pthread_cond_wait(&cond_,&mutex);
    }
    void wakeUp()
    {
        pthread_cond_signal(&cond);
    }
private:
    std::queue<Thread> threads_;
    std::queue<T> tasks_;
    pthread_mutex_t mutex_;
    pthraed_cond_t cond_; 
    int threadNum_;
    int taskNum_;
};
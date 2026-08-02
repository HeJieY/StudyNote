#pragma once 
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<iostream>
#include<pthread.h>
#include<queue>
#include<semaphore.h>
#include<ctime>
#include<unistd.h>
#include<vector>
#include<string.h>
#include<stdlib.h>

template <class T>
class RingQueue
{
    const static int MAX_SIZE = 10;
public:
    RingQueue():
    maxSize_(MAX_SIZE),rear_(0),front_(0),ringQueue_(MAX_SIZE)
    {
        sem_init(&producer_,0,maxSize_);
        sem_init(&consumer_,0,0);
        pthread_mutex_init(&mutex_,nullptr);
    }
    ~RingQueue()
    {
        sem_destroy(&producer_);
        sem_destroy(&consumer_);
        pthread_mutex_destroy(&mutex_);
    }
    void push(T val)
    {
        //申请一个信号量
        printf("push a request!\n");
        sem_wait(&producer_);
        pthread_mutex_lock(&mutex_);
        ringQueue_[front_] = val;
        front_ = (front_ +1) % maxSize_;
        pthread_mutex_unlock(&mutex_);
        sem_post(&consumer_);
    }

    T pop()
    {
        sem_wait(&consumer_);
        pthread_mutex_lock(&mutex_);
        T val = ringQueue_[rear_];
        rear_ = (rear_+1)%maxSize_;
        pthread_mutex_unlock(&mutex_);
        sem_post(&producer_);

        return val;
    }

private:
    std::vector<T> ringQueue_;
    int rear_;
    int front_;
    int maxSize_;
    sem_t consumer_;
    sem_t producer_;
    pthread_mutex_t mutex_;

};

//关于循环队列：
//1. 循环队列是指在有限的数组空间中不断就行覆盖，做到节省空间的队列
//2. 实现阻塞队列会面对一个问题，当read == front时有两种情况，此时可能队列是满的，也可能队列是空的
//3. 有两种比较可行的方法，第一，牺牲一个空间，此时循环队列满的条件变为（read+1）%maxSize == front
//4. 第二，引入第三方变量用来统计循环队列空间状况
//   在实现基于循环队列的cp模型时，因为第三方信号量存在，所以不用我们记录了
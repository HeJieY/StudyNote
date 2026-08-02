#pragma once
#include<cstdio>
#include<cstring>
#include<unistd.h>
#include<pthread.h>
#include<queue>
#include<ctime>
#include<cstdlib>
pthread_mutex_t lock;
template <typename T>
class BlockQueue
{
    static const int MAX_SIZE = 10;
    static const int WATER_HIGH = (MAX_SIZE*2)/3;   // 6
    static const int WATER_LOW = MAX_SIZE/3;//1     //3
public:
    BlockQueue(int maxSize = MAX_SIZE):
        _lock(&lock),
        _maxSize(maxSize)
    {
        pthread_mutex_init(&lock,nullptr);
        pthread_cond_init(&_consumer,nullptr);
        pthread_cond_init(&_producer,nullptr);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&_consumer);
        pthread_cond_destroy(&_producer);

    }

    bool lackResource()
    {
        if(_blockQueue.size() < WATER_LOW)
        {
            printf("%ld",_blockQueue.size());
            return true;
        }
        else if(_blockQueue.size()>WATER_HIGH)
        {
            printf("%ld",_blockQueue.size());
            return false;
        }
        return true;
    }

    bool isEmpty()
    {
        return _blockQueue.empty();
    }

    void push(T& val)
    {
        
        pthread_mutex_lock(_lock);
        while(!lackResource())//使用while是为了防止伪唤起   
        {
            printf("producer wait\n");
            pthread_cond_wait(&_producer,_lock);
        }
        printf("生产者生产了\n");
        _blockQueue.push(val);
        //一旦push了就代表可以呼唤消费者来消费了
        pthread_cond_signal(&_consumer);
        pthread_mutex_unlock(_lock);
    }

    T& pop()
    {
        pthread_mutex_lock(_lock);
        while(lackResource())//使用while是为了防止伪唤起   
        {
            printf("consumer wati\n");
            pthread_cond_wait(&_consumer,_lock);
        }
        T& data = _blockQueue.front();
        _blockQueue.pop();
        printf("消费者消费了\n");
        pthread_cond_signal(&_producer);
        pthread_mutex_unlock(_lock);
        return data;
    }

private:
    pthread_mutex_t* _lock;
    std::queue<T> _blockQueue;
    int _maxSize;
    pthread_cond_t _consumer;
    pthread_cond_t _producer;
};

class Cal
{
public:
    Cal()
    {
        srand(time(nullptr)^getpid());
        _x = rand()%10;
        _y = rand()%10;
        _op = getOpera(rand()%4+1);      
    }
    ~Cal()
    {

    }
public:
    int _x;
    int _y;
    char _op;
    int _ret;
    int _exitCode;
    char getOpera(char op)
    {
        switch(op)
        {
            case(1):
                return '+';
            case(2):
                return '-';
            case(3):
                return 'x';
            case(4):
                return '/';
        }
        return 0;
    }
};
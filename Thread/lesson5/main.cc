// 1. 信号量本质是一个计数器，用来描述临界资源的多少，它的PV操作是原子的，也就是它保证你申请资源和释放资源的操作是原子的
// 2. 当信号量为二元信号量时，天生就可以完成互斥操作，因为申请了一个资源就没有，后来者就会互斥了
// 3. 为什么不需要考虑临界资源计数问题，因为信号量是原子的，没有了自己会停下来
// 4. 就目前来看，信号量的优点是不需要cond系列也可以做到同步，还有可以限制访问量
#include "Task.hpp"
#include "RingQueue.hpp"

#define P 1
#define C 1
void *producer(void *args)
{        
    RingQueue<Task> *ringQueue = static_cast<RingQueue<Task> *>(args);
    
    while (1)
    {
        Task req;
        ringQueue->push(req);
        sleep(1);
        
    }

}

void *consumer(void *args)
{
    RingQueue<Task> *ringQueue = static_cast<RingQueue<Task> *>(args);
    while(1)
    {
        Task rsp = ringQueue->pop();
        rsp.getAnswer();
        sleep(1);
    }
}

int main()
{
   
    RingQueue<Task> *ringQueue = new RingQueue<Task>();
    std::vector<pthread_t> c, p;
    // 创建线程
    for (int i = 0; i < C; i++)
    {
        pthread_t c1;
        printf("消费者线程1已创建\n");
        pthread_create(&c1, nullptr, consumer, ringQueue);
        c.push_back(c1);
    }
    for (int i = 0; i < P; i++)
    {
        printf("生产者线程1已创建\n");
        pthread_t t1;
        pthread_create(&t1, nullptr, producer, ringQueue);
        p.push_back(t1);
    }
    // 回收线程

    for (int i = 0; i < C; i++)
    {
        pthread_join((c[i]), nullptr);
    }
    for (int i = 0; i < P; i++)
    {
        pthread_join(p[i], nullptr);
    }

    return 0;
}



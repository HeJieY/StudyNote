// 1. __thread 是一个编译选项，这种技术叫做线程的局部储存，它的作用是创建一个线程级别的全程变量：即是给某线程使用的私有全局变量，位置在局部存储空间
// 2. 线程有自己的栈空间，但事实上，这些是名义上独立的，但实际是共享区仍在私有空间，将地址抛出，其他线程便也能够使用这些变量了
// 3. 在Linux中的线程，其实应该轻量级进程+概念级线程
// 4. preate_detach 是线程分离技术，它的作用是将一个线程与主线程分离，当线程退出后，主线程会自动回收新线程
// 5. 要注意线程id和tid的区别，线程id是上层线程应用层的一个概念，使用pthread_self可以获得当前id值，gettid返回的是tid它是内核级概念，是管理轻量级线程的一个概念；
// 6. 在使用多线程时，由于存在cpu的线程切换概念，所以会出现一个数据可能在被多个变量读写，可能会导致数据不正常，我们可以使用互斥的概念来解决这个问题->也就是锁

#include <unistd.h>
#include <string>
#include <cstring>
#include <vector>
#include "LockGuard.hpp"
#define N 10
static int tickets = 1000;
Mutex mutex;
class ThreadDate
{
public:
    ThreadDate(const std::string &threadName) : _threadName(threadName)
    {
    }

    ~ThreadDate()
    {
    }

public:
    std::string _threadName;
    pthread_t _tid;
    int _sum;
};

void *func(void *args)
{

    ThreadDate *t = static_cast<ThreadDate *>(args);
    t->_tid = pthread_self();
    while (true)
    {
        {
            LockGuard LockGuard(&mutex); // RAII风格的锁，它是一个临时变量，出了空间后自动析构
            //这里尤其要注意不可是使用值拷贝，因为第一mutex传参会拷贝一次
            // 这是c++的代码块
            if (tickets > 0)
            {
                printf("%s get a tickets,tickets:%d\n", t->_threadName.c_str(), tickets);
                usleep(3000);
                tickets--;
                t->_sum++;
            }
            else
            {
                break;
            }
        } // 这里使用c++的代码块能够更明显的看出临界区
    }
    pthread_detach(pthread_self());
    return nullptr;
}

int main()
{
    int sum_tickets = 0;
    std::vector<ThreadDate *> v;
    // 创建三个线程
    for (int i = 0; i < N; i++)
    {
        ThreadDate *t = new ThreadDate("thread" + std::to_string(i + 1));
        pthread_t tid;
        pthread_create(&tid, nullptr, func, t);
        v.push_back(t);
    }

    sleep(10);

    for (auto &t : v)
    {
        sum_tickets += t->_sum;
        delete t;
    }

    printf("%d\n", sum_tickets);
    return 0;
}

// int main()
// {
//     std::vector<pthread_t> v;
//     for(int i=0;i<=N;i++)
//     {
//         ThreadDate* t = new ThreadDate("thread"+std::to_string(i+1));
//         pthread_t tid;
//         pthread_create(&tid,nullptr,func,t);
//         v.push_back(tid);
//     }
//     sleep(5);
//     for(int i=0;i<=N;i++)
//     {
//         int ret = pthread_join(v[i],nullptr);
//         if(ret != 0)
//         {
//             printf("%d",ret);
//         }

//     }
//      return 0;
// }
// void* func(void* args)
// {
//     ThreadDate* t = static_cast<ThreadDate*>(args);
//     printf("%u,%s,%p\n",gettid(),t->_threadName.c_str(),&t);
//     delete t;
//     sleep(1);
//     pthread_detach(pthread_self());
//     return nullptr;
// }

// 这个函数用于购买票
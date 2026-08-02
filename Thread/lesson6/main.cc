#include"Task.hpp"
void* fun1(void* args)
{
    Thread* t = static_cast<Thread*>(args);
    printf("I am thread %s\n",t->getName().c_str());
    sleep(1);
    return nullptr;
}


void* fun2(void* args)
{
    Thread* t = static_cast<Thread*>(args);
    printf("I am thread %s\n",t->getName().c_str());
    sleep(1);
    return nullptr;
}
std::vector<Thread> thread(5);
std::vector<Task> tasks(10);

int main()
{
    //模拟实现一个线程池，主线程生产任务，vector为共享空间，其他线程负责去拿任务
    
}
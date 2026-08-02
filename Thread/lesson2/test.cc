//1. pthread_create:用于创建一个线程
//2. pthread_join: 用于等待一个线程，并收取它的返回值吗，此外这个返回值可以传类结构体等等；
//3. pthread_cancal:用于取消一个线程,是由其他线程调用
//4. pthread_exit:用于取消一个线程，是由原线程自己调用
//5. pthread_self

#include<pthread.h>
#include<iostream>
#include<unistd.h>
using namespace std;
class Calac
{
public:
    Calac(int start,int end,const string& threadName):
        _start(start),
        _end(end),
        _threadName(threadName)
    {
        _answer = 0;
    }
    ~Calac()
    {

    }
void* test(void* test)
{

}
public:
    int _start;
    int _end;
    const string _threadName;
    int _answer;
};
void* countSum(void* arg)
{
    Calac* req = static_cast<Calac*>(arg);
    Calac* ret = new Calac(req->_start,req->_end,req->_threadName);
    for(int i=req->_start;i<=req->_end;i++)
    {
        ret->_answer+=i;
    }
    delete req;
    sleep(1);
    return ret;
}
int main()
{
    pthread_t tid;
    Calac* cal = new Calac(1,100,"thread1");
    // pthread_create(&tid,nullptr,cal->test,nullptr);不能传输一个成员函数，为什么？因为成员函数有一个隐藏的this指针；
    pthread_create(&tid,nullptr,countSum,cal);
    void** ret;
    pthread_join(tid,ret);//这里传的是二级指针
    Calac* answer = static_cast<Calac*>(*ret);
    sleep(1);
    cout << answer->_start<<" " << answer->_end <<" " << answer->_answer << endl;
    delete answer;

    return 0;

    
}
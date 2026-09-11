#pragma  once
#include<sys/epoll.h>

class Epoll
{
public:
    Epoll()
        :_epollFd(epoll_create1(0))
    {

    }
    ~Epoll()
    {

    }
    int  eventAdd()
    {

    }
    int eventDel()
    {
        
    }
    void eventCtl()
    {

    }

    int eventWait(struct epoll_event* events,const int& num,const int& timeout)
    {
       return epoll_wait(_epollFd,events,num,timeout);
    }

private:
    int _epollFd;

};
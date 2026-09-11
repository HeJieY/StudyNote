#pragma once
#include "log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
#include<sys/epoll.h>
#include<unordered_map>
#include<vector>
#include"Epoll.hpp"
#include<Connection.hpp>
#define NUM 1024
#define TIME_OUT 10000
class Reactor
{
public:
    Reactor()
        :_start(true)
        ,_epoll(std::make_unique<Epoll>())
    {
        
    }
    ~Reactor()
    {

    }

    void start()
    {
        while(_start)
        {
            loop();
        }
    }
    
    void loop()
    {
        dispatechEvents();
    }

    void dispatechEvents()
    {
        int ret = _epoll->eventWait(_revents,NUM,TIME_OUT);
        if(ret>=0)
        {
            for(int i=0;i<ret;i++)
            {
                //就绪事件,异常事件统一处理
                int socket = _revents[i].data.fd;
                uint32_t revent = _revents[i].events;
                if(revent &  EPOLLERR)
                    revent |= (EPOLLIN | EPOLLOUT);
                if(revent & EPOLLHUP)
                    revent |= (EPOLLIN | EPOLLOUT);
                if(revent & EPOLLIN)
                    _map[socket].recvMessage();
                if(revent & EPOLLOUT)
                    _map[socket].sendMessage();
            }
        }
        else if(ret == 0)
        {
            LOG(INFO,"timeout...");
        }
        else
        {
            LOG(ERROR,"Wait Error");
        }
    }
    bool addConnections(int fd,Connection& newConnection)
    {
        _map[fd] =  newConnection;
        
        _connections.push_back(newConnection);
        return true;
    }
private:

    std::unordered_map<int,Connection> _map;
    std::vector<Connection> _connections;
    std::unique_ptr<Epoll> _epoll;
    struct epoll_event _revents[NUM];
    bool _start;
};

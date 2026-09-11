#pragma once
#include"InetAddr.hpp"
#include"Reactor.hpp"
class Connection
{
public:
    Connection()
    {

    }
    ~Connection()
    {

    }
    virtual ssize_t sendMessage() = 0;
    virtual bool recvMessage() = 0;
    virtual int  getSocketFd() = 0;
    virtual std::string getClientData() = 0;
protected:
    int _socketFd;
    InetAddr _client;
    Reactor* _R; 
};



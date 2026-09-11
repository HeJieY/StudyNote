#pragma once
#include"Connection.hpp"
#include"log.hpp"
class Listen : public Connection
{
public:
    Listen()
    {

    }
    ~Listen()
    {

    }
    ssize_t sendMessage()
    {

    }
    
    bool recvMessage()
    {
       //到达这里说明事件已经准备就绪了,可以直接进行事件处理
        int newFd = accept(_socketFd,_client.getSockAddrIn(),_client.getLenAddress());
        LOG(INFO,"Get a new link!,socket is %d,addr:%s",_socketFd,_client.tostring().c_str());
        if(newFd < 0)  
        {
            LOG(INFO,"Accept error!"); 
            return false;
        }
        else
        {
            //接收并且添加到_map之中
            _R->addConnections(newFd,*this);
        }
        return true;

    }
protected:

};

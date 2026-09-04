
#include "log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
#include<sys/epoll.h>
#define NUM 1024
class EpollServer
{
public:
    EpollServer()
        :_listenSocket(std::make_shared<TcpSocket>()), _host("127.0.0.1", 8081)
        ,_start(true)
    {
        _epollFd = epoll_create1(0);
        if (_epollFd == -1)
        {

            printf("epollFd:%d\n", _epollFd);
            perror("epollFd create fail:");
        }
        _listenSocket->buildTcpSocket(_host);
        memset(_eEvents,0,sizeof(_eEvents));
        struct epoll_event eEvent;
        eEvent.data.fd = _listenSocket->getSocketFd();
        eEvent.events = EPOLLIN; 
        epoll_ctl(_epollFd,EPOLL_CTL_ADD,_listenSocket->getSocketFd(),&eEvent);
        _timeout = 1000;
        
    }
    ~EpollServer()
    {
        close(_listenSocket->getSocketFd());
    }

    void start()
    {
        while(_start)
        {
            int ret = epoll_wait(_epollFd,_eEvents,NUM,_timeout);
            if(ret > 0)
            {
                dispatchEvent(ret);
            }
            else if(ret ==  0)
            {
                LOG(INFO,"timeout...");
            }
            else
            {

                printf("%d\n",ret);
                sleep(2);
                LOG(INFO,"No event happen!");
                LOG(DEBUG,"%s",strerror(errno));
            }

        }
    }

protected:
    bool dealAccept()
    {
        LOG(INFO, "Listen event happen\n");
        // listen到了一个连接
        InetAddr client;
        int fd = _listenSocket->netAccept(client);
        addConnect(fd);
        // listen之后将该sockfd添加到维护数组中,并更新这个维护数组
        return true;
    }

    void dealRequest(int fd)
    {
        LOG(INFO, "Request event happen\n");
        // 已接受到报文可以处理事件
        char buffer[1024];
        memset(buffer,0,sizeof(buffer));
        ssize_t ret = recv(fd, buffer, sizeof(buffer), 0);
        if (ret <= 0)
        {
            LOG(INFO, "socketfd:%d,connect close!\n",fd);
            closeEpollFd(fd);
        }
        LOG(INFO, "socketFd:%d,recv message!\n", fd);
        std::string message = "Server echo#";
        message += buffer;
        ssize_t n = send(fd, message.c_str(), message.size(), 0);
        if (n <= 0)
        {
            LOG(INFO, "socketfd:%d,connect close!\n", fd);
            closeEpollFd(fd);
        }
        LOG(INFO, "socketFd:%d,send message!\n",fd);
    }

    void dispatchEvent(const int& eventNum)
    {
        for(int i=0;i<eventNum;i++)
        {
            if(_eEvents[i].data.fd == _listenSocket->getSocketFd())
            {
                dealAccept();
            }
            else
            {
                dealRequest(_eEvents[i].data.fd);
            }
        }
    }
    
  
    void closeEpollFd(int fd)
    {
        int ret = epoll_ctl(_epollFd,EPOLL_CTL_DEL,fd,nullptr);
        if(ret == 0)
        {
            LOG(INFO,"Delete success!Fd is: %d",fd);
        }
        else
        {
            LOG(INFO,"Delete error! Fd is: %d",fd);
        }
    }

    void addConnect(int fd)
    {
        struct epoll_event  eEvent;
        eEvent.events = EPOLLIN;
        eEvent.data.fd = fd;
        epoll_ctl(_epollFd,EPOLL_CTL_ADD,fd,&eEvent);
        LOG(INFO,"Add a fd: %d",fd);
    }

protected:
    std::shared_ptr<TcpSocket> _listenSocket;
    InetAddr _host;
    int _epollFd;
    bool _start;
    struct epoll_event _eEvents[1024];
    int _timeout;
};

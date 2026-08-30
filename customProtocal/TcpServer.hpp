#pragma once
#include"InetAddr.hpp"
#include"Socket.hpp"
#include<functional>
#include<signal.h>
using Handler_t = std::function<std::string(std::string&)>;
class TcpServer
{
public:
    TcpServer(std::string& ip,const uint16_t& port,Handler_t hander)
        :_hander(hander)
        ,_listenSocket(std::make_unique<TcpSocket>())
        ,_server(ip,port)
    {
        _listenSocket->buildTcpSocket(_server);
        LOG(INFO,"Create listen socket success!\n");
    }
    ~TcpServer()
    {
        _listenSocket->resourceClose();
    }

   
    
    void loop()
    {
        //系统默认对子进程的退出是忽略的(不作为),但是加上SIG_IG后,系统会优化子进程,自动回收僵尸进程 
        signal(SIGCHLD,SIG_IGN); 
        InetAddr client;
        while(true)
        {
            std::shared_ptr<Socket> socketFd = _listenSocket->netAccept(client);
            // auto socket =_listenSocket->netAccept(client);
            if(socketFd->getSocketFd() == -1)
                continue;
            LOG(INFO,"Get a new link,socketfd is %d\n",socketFd->getSocketFd());

            if(fork() == 0)
            {
                //子进程
                service(socketFd,client);
                socketFd->resourceClose();
                exit(0);

            }

        }

    }
private:
    InetAddr _server;
    std::unique_ptr<Socket> _listenSocket;
    Handler_t _hander; 
private:
    void service(std::shared_ptr<Socket> socketFd,const InetAddr& client)
    {
        std::string inBuffer,outBuffer;
        while(true)
        {
            inBuffer.clear();
            socketFd->netRecv(&inBuffer); 
            LOG(INFO,"Get client say# %s\n",inBuffer.c_str());
            
        }
    }
};
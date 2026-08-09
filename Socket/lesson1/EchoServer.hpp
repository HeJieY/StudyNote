#ifndef __ECHOSERVER__HPP
#define __ECHOSERVER__HPP
#include<sys/socket.h>
#include<iostream>
#include"log.hpp"
#include<memory>
#include<netinet/in.h>
#include<arpa/inet.h>
Log LOG; 
const static int defaultSocket = -1;
enum
{
    SUCESS = 0,
    SOCKET_ERROR,
    USAGE_ERROR,
    BIND_ERROR
};

class Server
{
public:
    Server(const std::string& ip,const uint16_t& port)
        :_socket(defaultSocket)
        ,_ip(ip)
        ,_port(port)
    {

    }
    ~Server()
    {
        close(_socket);
    }
    void init()
    {
        //1.创建初始化socket
        int socketFd = socket(AF_INET,SOCK_DGRAM,0);
        if(socketFd == -1)
        {
            LOG(FATAL,"socket套接字创建失败\n");
            exit(SOCKET_ERROR);
        }
        _socket = socketFd;
        LOG(INFO,"socket创建成功\n");
        //2.填充网络参数信息
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        // local.sin_addr.s_addr = inet_addr((_ip.c_str()));
        local.sin_addr.s_addr = INADDR_ANY;
        //INADDR_ANY是最佳实践,云服务器是禁止用户进行固定ip绑定的,考虑这种情况
        //当运行了一个进程之后,一台主机可能拥有多个ip地址(内网,外网,多网卡等等),那么这样一来
        //若这个进程单独绑定了一个ip地址,那么其他不同的ip地址便无法再对该进程进行访问
        //INADDR_ANY的含义就是将所有接收的报文的相同目的端口发向同一个进程
        local.sin_family = AF_INET;
        local.sin_port = htons(_port); 

        //3.设置到内核之中,通过系统提供的接口将网络参数信息填入;
        int n = bind(_socket,(sockaddr*)&local,sizeof(local));
        if(n < 0)
        {
            LOG(FATAL,"bind error!\n");
            exit(BIND_ERROR);
        }
        LOG(INFO,"Server init sucess!ip  is %s,port is %u\n",_ip.c_str(),_port);
        // std::cout << "bind success"  <<std::endl;
    }

    void  start()
    { 
        char inbuffer[1024];
        char outbuffer[1024];
        while(true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            ssize_t sz = recvfrom(_socket,inbuffer,sizeof(inbuffer),0,(struct sockaddr*)&peer,&len);
            //如果这里要提取对应的ip和port话,要用ntohs系列接口将网络序列转化为主机序列
            if(sz > 0)
            {
                inbuffer[sz] = 0;//把字符串最后的/0去掉
                LOG(DEBUG,"client say# %s\n",inbuffer);
                std::string echoString = "server say#";
                echoString+=inbuffer;
                sendto(_socket,echoString.c_str(),echoString.size(),0,(struct sockaddr*)&peer,len);
            }
            else
            {
                LOG(ERROR,"Receve error!\n");
            }
        }
    }


private:
    int _socket;
    const std::string _ip;
    const std::uint16_t _port;
};

#endif
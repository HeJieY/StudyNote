#pragma once
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include"log.hpp"
#include"InetAddr.hpp"
#include<memory>
extern Log LOG;
enum{
    SUCCESS = 0,
    SOCKET_INIT_ERROR,
    BIND_ERROR,
    LISTEN_ERROR
};
const static int gBackLog = 16;
//关于tcp连接的三个关键接口
//1.server端:listen accept
//listen:将被bind的socket设置为监听状态,持续捕捉
//accept:捕获监听状态的socket捕捉到的连接,并以套接字形式返回,这个socket就是与客户端通信的套接字
//2.client端:connect
//connect:相指定主机发送连接请求,三次握手成功后会该socket用于c/s通信
class  Socket
{
public:
   Socket() 
   {

   }
   //tcp
   virtual std::shared_ptr<TcpSocket>  netAccept(InetAddr& hostData) = 0;
   virtual int nettConnect() = 0;
   virtual ssize_t netRecv(std::string* out) = 0;
   virtual ssize_t netSendTo(const std::string& in) = 0;
   virtual bool netConnect(InetAddr& hostData) = 0;
   virtual void resourceClose() = 0;
   virtual bool buildTcpSocket(InetAddr& hostData) = 0;
   virtual int getSocketFd() = 0;
protected:
   virtual void createSocket() = 0;
   virtual void netBind(InetAddr& hostData);
   virtual bool netListen() = 0;
   //udp
protected:
};

class TcpSocket : public Socket
{
    public:
    bool netConnect(InetAddr& hostData) override
    {
        int ret = connect(_socketFd,hostData.getSockAddrIn(),hostData.getLen());
        if(ret == 0)
            return true;
        else
            return false;
    }
    std::shared_ptr<TcpSocket> netAccept(InetAddr& hostData) override
    {
        int condFd = accept(_socketFd,hostData.getSockAddrIn(),hostData.getLenAddress());
        //accept函数参数介绍
        //param1:监听套接字
        //param2,3:带出客户端的信息
        if(condFd ==-1)
        {
            LOG(WARN,"Accept error!\n");
            return nullptr;
        }
        else
        {
            return std::make_shared<TcpSocket>(condFd);
        } 

    }
    bool buildTcpSocket(InetAddr& hostData)
    {
        createSocket();
        netBind(hostData);
        netListen();
    }
    ssize_t netSendTo(const std::string& in) override
    {
        ssize_t ret = send(_socketFd,in.c_str(),in.size(),0);
        if(ret >= 0)
        {
            LOG(INFO,"Send success!\n");
        }
        else
        {
            LOG(WARN,"Send error!\n");
        }
    }

    ssize_t netRecv(std::string* out) override
    {
        char buffer[1024];
        ssize_t ret = recv(_socketFd,buffer,sizeof(buffer)-1,0);
        if(ret >= 0)
        {
            LOG(INFO,"Recv Success!\n");
            buffer[ret] = 0;
            *out += buffer;
            return ret;
        }
        else
        {
            LOG(WARN,"Recv error!\n");
        }
    }
    int getSocketFd() override
    {
        return _socketFd;
    }
    void resourceClose() override
    {
        close(_socketFd);
    }
protected:
    void createSocket() override
    {
       _socketFd = socket(AF_INET,SOCK_STREAM,0);
       if(_socketFd < 0)
       {
            LOG(FATAL,"Socket init,error!\n");
            exit(SOCKET_INIT_ERROR);
       }

    }
    void netBind(InetAddr& hostData) override
    {
       int ret = bind(_socketFd,hostData.getSockAddrIn(),hostData.getLen());
        if(bind < 0)
        {
            LOG(ERROR,"Bind error!\n");
            exit(BIND_ERROR);
        }
    }
   bool netListen() override
   {
       if(listen(_socketFd,gBackLog)!=0)
       {
            LOG(ERROR,"Listen error!\n");
       }
    
   }
private:
   int _socketFd;

};

class UdpSocket
{



};
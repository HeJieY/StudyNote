#pragma once
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
//这个类使用来描述设备网络信息的
//1.提供sockaddr_in 提取具体的信息
//2.提供ip,port 构造对应得sockaddr
class InetAddr
{
public:
    InetAddr()
    {
        
    }
    InetAddr(const std::string& ip,const uint16_t port)
        :_ip(ip)
        ,_port(port)
    {
    
        inet_pton(AF_INET,_ip.c_str(),&(_host.sin_addr));
        //inet_ntop用于将二进制转化为点分十进制,缓冲区需要自己提供,并且是线程安全得
        _host.sin_port =  htons(_port);
    //    ntohs这个用来网络序列转化为主机序列
        _host.sin_family  = AF_INET;
        _len = sizeof(_host);
    }

    InetAddr(const struct sockaddr_in& host)
    {
        char buffer[32];
        inet_ntop(AF_INET,&host.sin_addr,buffer,sizeof(host));
        _host = host;
        _ip =  buffer;
        _len  = sizeof(host);
        _port = ntohs(host.sin_port);
        
    }
    struct sockaddr* getSockAddrIn()
    {
        return (struct sockaddr*)&_host;
    }
    const std::string tostring()
    {
        //此外这里返回的是一个字符串,出了作用域会自动销毁的,不可以用引用
        //c++11之后返回局部对象编译器会进行优化,把局部对象当成右值
        return "IP: "+_ip+",Port: "+std::to_string(_port)+"\n";

    }

    bool operator==(const struct InetAddr& client)
    {
        return (client._ip == _ip) && (client._port == _host.sin_port);
    }

    bool operator=(const struct InetAddr& client)
    {
        _ip = client._ip;
        _port = client._port;
        _host =  client._host;
        _len = client._len;
    }
    socklen_t* getLenAddress()
    {
        return &_len;
    }
    socklen_t getLen()
    {
        return _len;
    }
    
    ~InetAddr()
    {

    }
private:
    
    struct sockaddr_in _host;
    socklen_t _len;
    uint16_t _port;
    std::string _ip;
};


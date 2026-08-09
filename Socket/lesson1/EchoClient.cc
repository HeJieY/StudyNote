#include<iostream>
#include<string>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<netinet/in.h>
void tip(int& argc)
{
    if(argc != 3)
    {
        std::cout << "tips" << std::endl;
        exit(-1);
    }

}
int main(int argc,char* argv[])
{

    tip(argc);
    int socketFd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketFd < 0)
    {
        std::cout<<"socket inint error" << std::endl;
        exit(-1);
    }  
    //客户端需要有自己的发送ip和端口,但是它们不需要我们进行固定绑定
    while(true)
    {
        std::cout << "请输入你数据# " <<  std::endl;
        std::string message;
        std::getline(std::cin,message);
        struct sockaddr_in  serverInfo;
        bzero(&serverInfo,sizeof(serverInfo)); 
        serverInfo.sin_family = AF_INET;
        serverInfo.sin_addr.s_addr = inet_addr(argv[1]);
        serverInfo.sin_port = htons(std::stoi(argv[2]));
        int n = sendto(socketFd,message.c_str(),message.size(),0,(sockaddr*)&serverInfo,sizeof(serverInfo));
        if(n > 0)
        {
            //读消息
            char inBuffer[1024];
            struct sockaddr_in server;
            socklen_t len = sizeof(server);
            ssize_t sz = recvfrom(socketFd,inBuffer,sizeof(inBuffer),0,(sockaddr*)&server,&len);
            if(sz > 0) 
            {
               std::cout << inBuffer << std::endl; 
            }
        }
    }
       return 0;
}
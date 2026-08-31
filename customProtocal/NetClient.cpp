#include"Socket.hpp"
#include"log.hpp"
#include"Protocol.hpp"
#include"InetAddr.hpp"
#include<memory>
int main()
{
    
    std::shared_ptr<TcpSocket> tcpClient = std::make_shared<TcpSocket>();
    InetAddr clientInfo("127.0.0.1",8081);
    int  cnt = 3;
    while(cnt--)
    {
        int ret = tcpClient->netConnect(clientInfo);
        if(ret == -1)
        {
            LOG(ERROR,"Connect server fail.....\n");
            perror("connect fail"); 
            sleep(3);
        }
        else
        {
            std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>([](Response& rsp)->void{
                std::cout << rsp._result <<" " << rsp._status << std::endl;
            });
            int x,y;
            char op;
            LOG(INFO,"Enter your x#");
            std::cin >> x;
            LOG(INFO,"Enter your y#");
            std::cin >> y;
            LOG(INFO,"Enter your op#");
            std::cin  >> op;
            //协议与打包
            Request req(x,y,op);
            std::string jsonStr;
            req.serialize(&jsonStr);
            protocol->packet(jsonStr);
            int ret = tcpClient->netSendTo(jsonStr);
            if(ret < 0) 
            {
                LOG(DEBUG,"Send error!\n");
            }
            //接收返回的报文
            std::string in;
            while (true)
            {
                tcpClient->netRecv(&in);
                protocol->parseResponse(in);
                if(in =="")
                    continue;
            }
        }
    }
}
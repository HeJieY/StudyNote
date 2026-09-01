#include"Protocol.hpp"
#include"Socket.hpp"
#include"TcpServer.hpp"
#include"Calculator.hpp"
int main()
{   
    std::unique_ptr<Calculator> calculator = std::make_unique<Calculator>();
    std::unique_ptr<Protocol> protocol = std::make_unique<Protocol>([&calculator](Request& req)->Response{
        // LOG(INFO,"Protocol request hander used\n");
        return calculator->Execute(req);
    });
    std::unique_ptr<TcpServer> tcpServer = std::make_unique<TcpServer>("127.0.0.1",8081
    ,[&protocol](std::string& inbuffer)->std::string{
        // LOG(INFO," TcpServer hander used\n");
       return protocol->parseRequest(inbuffer);
    });

    tcpServer->loop();
    return 0;
}
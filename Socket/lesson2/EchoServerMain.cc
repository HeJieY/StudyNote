#include"EchoServer.hpp"

void  tip(const int& argc)
{
    if(argc!= 3)
    {
        std::cout << "Use tip:.exe ip port" << std::endl;
        LOG(FATAL,"IP or Port Error!\n");
        exit(USAGE_ERROR);
    }

}
int main(int argc,char* argv[])
{
    tip(argc); 
    Dict dict;
    std::unique_ptr<Server> server = std::make_unique<Server>([&dict](const std::string& word)->const std::string{
        return dict.translate(word);
    },argv[1],std::stoi(argv[2]));
    server->init();
    server->start();
    return 0;

}
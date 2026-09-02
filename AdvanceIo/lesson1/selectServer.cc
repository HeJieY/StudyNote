#include"selectServer.hpp"


int main()
{
    std::unique_ptr<SelectServer> selectServer =  std::make_unique<SelectServer>();
    selectServer->start();
    return 0;
}
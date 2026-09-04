#include"EpollServer.hpp"


int main()
{
    std::unique_ptr<EpollServer> pollServer = std::make_unique<EpollServer>();
    pollServer->start();
    return 0;
}
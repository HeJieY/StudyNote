#include"pollServer.hpp"


int main()
{
    std::unique_ptr<PollServer> pollServer = std::make_unique<PollServer>();
    pollServer->start();
    return 0;
}
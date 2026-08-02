#include "log.hpp"

int main()
{
    Log log;
    log.screenPrint(INFO,"nihao");

    log.screenPrint(DEBUG,"nihao");
    log.screenPrint(WARN,"nihao");
    log.screenPrint(ERROR,"nihao");
    sleep(5);
    log.screenPrint(FATAL,"nihao");
    log.screenPrint(INFO,"nihao");
    return 0;
}
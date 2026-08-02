#pragma once
#include<iostream>
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<cstring>

#define BUFFER_SIZE 128
#define DEFAULT_PATH "./myNamePipe"
using namespace std;
class pipeInit
{
public:
pipeInit(const std::string& path = DEFAULT_PATH,mode_t mode = 0666):
    _path(path),
    _mode(mode)
{

}
~pipeInit()
{

}
void init()
{
    int ret = mkfifo(_path.c_str(),_mode);
    if(ret == -1)
    {
        perror("mkfifo fail!");
        exit(-1);
    }
}

void clear()
{
    unlink(_path.c_str());
    printf("Delete namePipe!\n");
}
private:
    std::string _path;
    mode_t _mode;
};
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
//注意在设置非阻塞io中有些返回-1的情况不一定是错误要进行特殊处理

int main()
{
    int x;
    fcntl(0, F_SETFL, O_NONBLOCK);
    while (true)
    {
        char buffer[1024] = {0};
        ssize_t ret = read(0, buffer, sizeof(buffer));
        if (ret <= 0)
        {
            std::string s = "scanf error!\n";
            write(1,s.c_str(),s.size());
            sleep(2);
            continue;
        }
        std::string s = "scanf success:";
        s+=buffer;
        s+="\n";
        write(1,s.c_str(),s.size());
    }

    return 0;
}
//1. 信号的保存实际上是在对应的DS中对位图修改01操作，所以发信号实际是写信号
//2. 信号管理的内核中存在三个表，pending   block    handler
//   block：阻塞表，又叫信号屏蔽字表，这个表通过01来表示对某个信号是否进行阻塞
//   pending：位决信号位图，这个表用来保存进程没有处理的信号，对对应的位01表示信号是否已存在，此外，linux规定，1~31号信号在递达之前产生多次也只记录一次，实时信号不计；
//   handler：信号动作处理表，是函数指针数组，对信号进行处理
//3. 信号递达：处理信号的动作
//4. 信号未决：处于peeding表中的信号，已收到但未处理

#include<stdio.h>
#include<unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include<signal.h>
#include<iostream>
void Print(sigset_t peeding)
{
    for(int i=1;i<=31;i++)
    {
        if(sigismember(&peeding,i))
        {
            std::cout << "1";
        }
        else
        {
            
            std::cout << "0";
        }
        
    }
    printf("\n");
}
int main()
{
    sigset_t newSet,oldSet;//这是一个信号集，是由os提供的，他和sigpromask共同配置用来访问修改内核的三个表
    sigemptyset(&newSet);//清空信号集合
    sigemptyset(&oldSet);
    sigaddset(&newSet,2);//用于将某个信号屏蔽设置进入信号集

    sigprocmask(SIG_SETMASK,&newSet,&oldSet);//这个也是系统调用接口，第一个参数为使用什么方式。他与信号集搭配使用来控制三个表
    sigset_t peeding;
    while(1) 
    {
        sigpending(&peeding);//获得peeding表
        Print(peeding);
        sleep(1);
    }
    return 0;
}
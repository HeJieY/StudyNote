//1. volatile 是一个关键字，它的作用是保证内存的可见性
//2. 可重入函数：
//       场景：当我们在使用头插法给链表增加数据的时候，譬如走到了，p->next = head; 下一步应该为 p = head; 
//存在一种情况，当操作系统停在p->next执行完毕时，进程切换了，待到下一次，进程切换回来，os获取上下文，也就是从内核态进入用户态，此时会处理信号，那么如果自定义函数是一个
//链表的头插法，那么此时insert完成了，回到用户态，执行上一次代码的下一句，可以发现，内存泄漏了，丢掉了一个节点
//这种不可以被多个执行流同时进入的叫做不可重入函数，相对的就是可重入函数了；

#include<cstdio>
#include<signal.h>
#include<iostream>
using namespace std;
int flag = 0;
void handler(int signal)
{
    cout << "catch a signal" <<endl;
    flag = 1;
}

int main()
{
    signal(2,handler);
    while(!flag)
    sleep(1);

    //编译器会将flag优化进入寄存器
    
    return 0;
}
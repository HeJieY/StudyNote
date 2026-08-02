//1. Linux在信号递达时，会在切入用户态之前将pending表更改为1
//2. Os不准在执行某信号时被该信号再次递达也就是会先屏蔽，防止重复调用

#include <signal.h>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
// 这个函数用于检查信号是否被pending
void printPending()
{
    while (1)
    {
        sigset_t set; // 创建一个信号集
        sigpending(&set);

        for (int i = 1; i <= 31; i++)
        {
            if (sigismember(&set, i))
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
        printf("\n");
        sleep(1);
    }
}

void hander(int signal)
{
    printf("catch a singal:%d \n", signal);
    if (signal == 4)
    {
        printf("block the singal\n");
    }
    printPending();
}
int main()
{
    struct sigaction act, oact;
    memset(&act, 0, sizeof(act));
    memset(&oact, 0, sizeof(act));
    sigemptyset(&act.sa_mask); // 清空信号集合
    sigaddset(&act.sa_mask, 4);//sa_mask 这个和一般的阻塞不一样，它的阻塞指的是，当进程运行时，在执行某个信号的自定义代码时，顺带屏蔽掉某些信号
    act.sa_handler = hander; // 这是一个函数指针成员，将自定义的函数赋给
    sigaction(2, &act, &oact);

    while (1)
    {
        printf("My pid is %d \n", getpid());
        sleep(2);
    }

    return 0;
}
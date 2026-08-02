// 1. 当子进程运行完毕后退出会给父进程发送一个信号，SIGCHLD，17号
// 2. 该信号的默认处理方法是SIGDFL，当我们对该信号处理，使用默认SIGIGN时，父进程会回收子进程，但是无法记录子进程的退出信息
// 3. 我们可以使用wait和子进程发送信号的方法来对子进程回收，这样当信号来的时候在进行处理，主进程就可以不用一直等待（但是，前提是父进程要一直运行，否则会产生僵尸进程）
#include <cstdio>
#include <signal.h>
#include <cstring>
#include <sys/types.h>
#include <wait.h>
#include<time.h>
#include<stdlib.h>
void handler(int signal)
{
    int status;
    if (signal == 17)
    {
        printf("catch a signal 17\n");

        while (1)
        {
            sleep(2);
            int ret = waitpid(-1, &status, WNOHANG);
            //这里对该进程的返回情况，做一下捕捉
            printf("Exit code is: %d,Get signal is: %d,my pid is %d\n",(status)>>8&0xFF,(status)&0x7F,ret);
            if (ret > 0)
                continue;
            if (ret == 0)
            {
                printf("No process exit!\n");
                break;
            }
            if (ret < 0)
            {
                perror("wati fail");
                break;
            }
        }
    }
    if(signal == 2)
    {
        printf("catch a signal 2\n");
    }
}
int main()
{
    srand((unsigned)time(NULL)); 
    struct sigaction act, oact; // 一个结构体，里面提供了许多成员可以使用，让我们更好的捕捉信号
    memset(&act, 0, sizeof(act));
    memset(&oact, 0, sizeof(oact));
    // 我们还想要顺带阻塞一下2号信号
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,2);
    act.sa_handler = handler; //注意这个函数编译器不会提示
    //这个是处理函数
    sigaction(17,&act,&oact);
    // sigaction(2,&act,&oact);
    int cnt = 6;
    while(--cnt)
    {
        printf("I am father\n");
        int ret = fork();
        if(ret == 0)
        {
            //child
            sleep(rand()%5+1); //生成·1~5的随机数字
            exit(0);
        }
        sleep(1);
    }
    printf("父进程要走完了\n");
    sleep(5);
    return 0;
}
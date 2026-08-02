//  pipe的作用是建立了一个管道,管道可以看出是一个系统缓冲区，两个fd之间原本是没有关系的，需要管道来建立关系
//  本本案例中，进程池形成了一种这样的关系
//    fd4->pipe<-fd3        fd5->pipe<-fd3这样的关系

// dup 的作用是将第一个fd拷贝到第二个fd，并且关闭第二个fd，此外，fd的拷贝就像是这样，譬如，0，是标准输入流，也就是键盘，可以看作fd0记录了一个从键盘拿信息到文件中的方法，而我们创建的fd3
// 相当于记录了一个从fd4拿信息的方法，此时进行dup，相当于把fd0这条路径关闭了，fd3的信息写入了fd0，此时所有原先标准输入都会向fd0打

//TODO:这里有一个问题为什么在父进程关闭子进程的写端时，子进程没有立马关闭，而是等待父进程关闭后在关闭呢？
#include "Task.hpp"
class childProcess
{
public:
    childProcess(int processFd, const std::string &processName, int processId) : 
    _processFd(processFd), // 这个描述的是父进程向哪一个fd里写                                                        
    _processName(processName),
    _processId(processId)
    {
    }
    ~childProcess()
    {
    }

public:
    int _processFd;
    std::string _processName;
    int _processId;
};

void slaver()
{
    // 要让子进程从0区读数据，当父进程为派发任务的时候，子进程会阻塞，等待父亲的指令
    while (1)
    {

        char buffer[BUFFER_SIZE];
        buffer[0] = 0;
        printf("子进程应该要阻塞了\n");
        ssize_t child = read(0, buffer, sizeof(int));
        if (child <= 0)
        {
            printf("子进程退出了\n");
            exit(0);
        }
        if (child > 0)
        {
            printf("子进程读到的数据是：%d \n", atoi(buffer));
            tasks[atoi(buffer)]();
        }
    }
}

void initProcessPool(std::vector<childProcess> *vec)
{
    printf("init begin:\n");
    for (int i = 0; i < N; i++)
    {
        int pipeFd[2];
        int ret = pipe(pipeFd);
        if (ret == -1)
        {
            perror("pipe fail!");
            exit(-1);
        }

        int childId = fork();
        if (childId < 0)
        {
            perror("fork fail:");
            exit(-1);
        }
        if (childId == 0)
        {
            printf("子进程创建了哦,%d\n", i);
            // child :子进程作为读方关闭写区，并重定向
            close(pipeFd[1]);
            dup2(pipeFd[0], 0);
            close(pipeFd[0]);
            slaver();
        }
        // father:将子进程给添加到vec中；
        close(pipeFd[0]);
        std::string name = "hello I am child ,fd: ";
        name.append(std::to_string(pipeFd[1])).append("my pid is: ").append(std::to_string(childId));
        vec->push_back(childProcess(pipeFd[1], name, i));
    }
}

void cleanProcessPool(const std::vector<childProcess> &vec)
{
    printf("进入到clean函数了\n");
    for (auto &child : vec)
    {
        printf("进入循环了哦\n");
        // 测试进程是否创建完毕
        std::cout << child._processFd << "," << child._processName << std::endl;
        // 清理进程
        close(child._processFd);
        std::cout << "文件描述符关闭了: " << child._processFd << std::endl;
        sleep(2);
    }
    sleep(3);
    for (int i = 0; i < 8; i++)
    {
        wait(NULL);
        sleep(2);
    }
}

void controlTask(const std::vector<childProcess> &vec)
{
    // 创造一个随机数
    srand((unsigned)time(NULL));
    int cnt = 5;
    while (--cnt)
    {
        int n = rand() % 4 + 1;
        printf("创建的随机数是：%d\n", n);
        for (auto &process : vec)
        {

            if (process._processId == n)
            {
                write(process._processFd, std::to_string(n).c_str(), sizeof(int));
            }
        }
        sleep(5);
    }
}

int main()
{
    std::vector<childProcess> vec; // 定义了一个栈上的vector来管理类，实际上是申请了堆区管理类，也就是说它的生命周期不再回因为在函数调用而销毁
    // 0.加载任务
    loadTask(&tasks);
    // 1.创建进程池
    initProcessPool(&vec);
    // 2.分配任务
    controlTask(vec);

    // 3.清理进程池
    cleanProcessPool(vec);
    return 0;
}
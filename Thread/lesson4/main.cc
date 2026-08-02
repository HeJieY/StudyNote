#include "BlockQueue.hpp"

void *consumer(void *args)
{
    while(1)
    {
        sleep(3);
        BlockQueue<Cal>* blockQueue = static_cast<BlockQueue<Cal>*>(args);\
        Cal ans = blockQueue->pop();
        printf("I get a task!\n");
        //进行数据处理
        switch(ans._op)
        {
            case('+'):
                printf("x+y=%d\n",ans._x+ans._y);
                break;
            case('-'):
                printf("x-y=%d\n",ans._x-ans._y);
                break;
            case('*'):
                printf("x*y=%d\n",ans._x*ans._y);
                break;
            case('/'):
                if(ans._y == 0)
                {
                    printf("y is 0 !\n");
                    break;                    
                }
                printf("x/y=%d",ans._x/ans._y);
                break;
        }
    }
}
void *producer(void *args)
{
    while(1)
    {
        //获取资源用
        Cal req;
        BlockQueue<Cal>* blockQueue = static_cast<BlockQueue<Cal>*>(args);
        blockQueue->push(req);
        sleep(1); 
    }
}

int main()
{

    BlockQueue<Cal> blockQueue;
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, consumer, &blockQueue);
    pthread_create(&t2, nullptr, producer, &blockQueue);

    pthread_join(t1,nullptr);
    pthread_join(t2,nullptr);

    return 0;
}
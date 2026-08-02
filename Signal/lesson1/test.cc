#include<signal.h>
#include<stdio.h>
#include<unistd.h>
void sighandler(int signal)
{
    printf("I get a signal\n");
}

int main()
{       
    signal(2,sighandler);
    while(1)
    {
        printf("hello world\n");
        sleep(2);
    }   
    return 0;
}

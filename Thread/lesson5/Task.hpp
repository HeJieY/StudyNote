
#include"RingQueue.hpp"

class Task
{
public:
    Task()
    {
        srand(time(nullptr));
        x_ = rand()%9;
        y_ = rand()%9;
        op_ = getOpera(rand()%4+1);
        ans_ = 0;
        exitcode_ = 0;
    }
    ~Task()
    {

    }

    
public:
    int x_;
    int y_;
    int ans_;
    char op_;
    int exitcode_;
    void getAnswer()
    {
        switch(op_)
        {
            case('+'):
                printf("x+y=%d\n",x_+ y_);
                break;
            case('-'):
                printf("x-y=%d\n",x_-y_);
                break;
            case('*'):
                printf("x*y=%d\n",x_ * y_ );
                break;
            case('/'):
                if(y_ == 0)
                {
                    printf("y is 0 !\n");
                    break;                    
                }
                printf("x/y=%d",x_ /y_ );
                break;
        }
    }
    char getOpera(char op)
    {
        switch(op)
        {
            case(1):
                return '+';
            case(2):
                return '-';
            case(3):
                return 'x';
            case(4):
                return '/';
        }
        return 0;
    }
};
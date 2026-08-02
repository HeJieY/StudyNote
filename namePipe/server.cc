#include "namePipe.hpp"

int main()
{
    pipeInit namePipe;
    namePipe.init();
    int readFd = open(DEFAULT_PATH, O_RDONLY);
    while (1)
    {
        char buffer[BUFFER_SIZE];
        buffer[0] = 0;
        if (readFd == -1)
        {
            perror("open fail");
            exit(-1);
        }
        ssize_t n = read(readFd, buffer, sizeof(buffer));
        if(n <= 0)
        {
            exit(0);
        }
        else
        {
            cout << "clind say# " << string(buffer).c_str() << endl;
        }
    }
    close(readFd);
    return 0;
}
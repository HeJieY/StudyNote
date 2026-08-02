#include "namePipe.hpp"

int main()
{

    int writeFd = open(DEFAULT_PATH, O_CREAT | O_WRONLY);
    while(1)
    {
        if (writeFd == -1)
        {
            exit(-1);
        }
        while (1)
        {
            char buffer[BUFFER_SIZE];
            buffer[0] = 0;
            string userIn;
            cin.getline(buffer, BUFFER_SIZE);
            write(writeFd, buffer, strlen(buffer));
        }
    }
    close(writeFd);
    return 0;
}
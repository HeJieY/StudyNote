#include "log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
#include <sys/select.h>
static int gFdNum = 0;
const static int DEFAULT_FD_NUM = 0;
class SelectServer
{
public:
    SelectServer()
        : _listenSocket(std::make_shared<TcpSocket>()), _host("127.0.0.1", 8081)
    {
        memset(_readFds, 0, sizeof(_readFds));
        _listenSocket->buildTcpSocket(_host);
    }
    ~SelectServer()
    {
        close(_listenSocket->getSocketFd());
    }

    void start()
    {
        fd_set readFd;
        _readFds[0] = _listenSocket->getSocketFd();
        gFdNum++;
        _maxFd = _readFds[0];

        while (true)
        {
            struct timeval timeout{3, 0};
            // 重新维护fd_set
            keepReadFds(readFd);
            dispatchEvent(readFd, timeout);
        }
    }

protected:
    bool dealAccept()
    {
        LOG(INFO, "Listen event happen\n");
        // listen到了一个连接
        InetAddr client;
        int fd = _listenSocket->netAccept(client);
        // listen之后将该sockfd添加到维护数组中,并更新这个维护数组
        keepReadFds(fd);
        return true;
    }

    bool dealRequest(fd_set &readFd)
    {
        for (int i = 0; i < gFdNum; i++)
        {
            LOG(INFO, "Request event happen\n");
            if (FD_ISSET(_readFds[i], &readFd) && _readFds[i] != _listenSocket->getSocketFd())
            {
                // 已接受到报文可以处理事件
                char buffer[1024];
                ssize_t ret = recv(_readFds[i], buffer, sizeof(buffer), 0);
                if (ret <= 0)
                {
                    LOG(INFO, "socketfd:%d,connect close!\n", _readFds[i]);
                    gFdNum--;
                    close(_readFds[i]);
                    _readFds[i] = DEFAULT_FD_NUM;
                }
                LOG(INFO, "socketFd:%d,recv message!\n", _readFds[i]);
                std::string message = "Server echo#";
                message += buffer;
                ssize_t n = send(_readFds[i], message.c_str(), message.size(), 0);
                if (n <= 0)
                {
                    LOG(INFO, "socketfd:%d,connect close!\n", _readFds[i]);
                    gFdNum--;
                    close(_readFds[i]);
                    _readFds[i] = DEFAULT_FD_NUM;
                }
                LOG(INFO, "socketFd:%d,send message!\n", _readFds[i]);
            }
        }
    }
    void dispatchEvent(fd_set &readFd, struct timeval &timeout)
    {
        int ret = select(_maxFd + 1, &readFd, nullptr, nullptr, &timeout);
        if (ret == 0)
        {
            // 表示select超时返回
            LOG(WARN, "Select timeout...\n");
        }
        else if (ret < 0)
        {
            LOG(WARN, "Select error...\n");
        }
        else
        {
            LOG(INFO, "Event happen!\n");
            if (FD_ISSET(_listenSocket->getSocketFd(), &readFd) != 0)
            {
                dealAccept();
            }
            else
            {
                dealRequest(readFd);
            }
        }
    }
    void keepReadFds(fd_set &readFd)
    {
        int i = 0, flag = 0;
        FD_ZERO(&readFd);
        while (true)
        {
            if (flag == gFdNum)
            {
                break;
            }
            if (_readFds[i] != DEFAULT_FD_NUM)
            {
                FD_SET(_readFds[i], &readFd);
                flag++;
            }
            i++;
        }
    }
    void keepReadFds(int fd)
    {
        // 继续fd的添加
        for (int i = 0;; i++)
        {
            if (_readFds[i] != DEFAULT_FD_NUM)
            {
                _readFds[i] = fd;
                gFdNum++;
                break;
            }
        }
        _maxFd = _readFds[0];
        int i = 0, flag = 0;
        while (true)
        {
            if (flag == gFdNum)
            {
                break;
            }
            if (_maxFd < _readFds[i] && _readFds[i] != DEFAULT_FD_NUM)
            {
                _maxFd = _readFds[i];
                flag++;
                continue;
            }
            if (_readFds[i] != DEFAULT_FD_NUM)
            {
                flag++;
            }
            i++;
        }
    }

protected:
    std::shared_ptr<TcpSocket> _listenSocket;
    InetAddr _host;
    int _readFds[sizeof(fd_set) * 8];
    int _maxFd;
};
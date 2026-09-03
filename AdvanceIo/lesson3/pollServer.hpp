
#include "log.hpp"
#include "Socket.hpp"
#include "InetAddr.hpp"
#include <poll.h>
class PollServer
{
public:
    PollServer()
        : _listenSocket(std::make_shared<TcpSocket>()), _host("127.0.0.1", 8081)
    {
        _timeout = 1000;
        _size = 1;
        _listenSocket->buildTcpSocket(_host);
        _capactiy = 4;
        _fds = (struct pollfd *)malloc(sizeof(struct pollfd) * 4 * _capactiy);
        if(_fds  == nullptr)
        {
            perror("malloc fail");
            exit(-1);
        }
        for(int i=0;i<_capactiy;i++)
        {
            _fds[i].fd = -1;
            _fds[i].events = 0;
            _fds[i].revents  = 0;
        }
        _fds[0].fd = _listenSocket->getSocketFd();
        _fds[0].events = POLLIN | POLLRDHUP;
    }
    ~PollServer()
    {
        close(_listenSocket->getSocketFd());
    }

    void start()
    {
        while (true)
        {
            if (_fds == nullptr || isFull())
            {
                reSize();
            }
            int ret = poll(_fds, _size, _timeout);
            if (ret == 0)
            {
                //
                LOG(INFO, "wati timeout!\n");
                continue;
            }
            else if (ret > 0)
            {
                // 有事件发生处理事件
                dispatchEvent();
            }
            else if (ret == EINTR)
            {
                LOG(INFO, "Accept signal fail\n");
                continue;
            }
            else
            {
                LOG(DEBUG, "Poll error!\n");
                break;
            }
        }
        LOG(ERROR, "Server fail!\n");
        exit(-1);
    }

protected:
    bool dealAccept()
    {
        LOG(INFO, "Listen event happen\n");
        // listen到了一个连接
        InetAddr client;
        int fd = _listenSocket->netAccept(client);
        addConnect(fd);
        // listen之后将该sockfd添加到维护数组中,并更新这个维护数组
        return true;
    }

    void dealRequest(struct pollfd &fd)
    {
        LOG(INFO, "Request event happen\n");
        // 已接受到报文可以处理事件
        char buffer[1024];
        ssize_t ret = recv(fd.fd, buffer, sizeof(buffer), 0);
        if (ret <= 0)
        {
            LOG(INFO, "socketfd:%d,connect close!\n", fd.fd);
            closePollFd(fd);
        }
        LOG(INFO, "socketFd:%d,recv message!\n", fd.fd);
        std::string message = "Server echo#";
        message += buffer;
        ssize_t n = send(fd.fd, message.c_str(), message.size(), 0);
        if (n <= 0)
        {
            LOG(INFO, "socketfd:%d,connect close!\n", fd.fd);
            closePollFd(fd);
        }
        LOG(INFO, "socketFd:%d,send message!\n", fd.fd);
    }

    void dispatchEvent()
    {
        for (int i = 0; i < _size; i++)
        {
            if (_fds[i].fd != -1 && ((_fds[i].revents & POLLIN) || _fds[i].revents & POLLRDHUP))
            {
                if (_fds[i].fd != _listenSocket->getSocketFd())
                {
                    dealRequest(_fds[i]);
                }
                else
                {
                    dealAccept();
                }
            }
        }
    }

    void reSize()
    {
        struct pollfd *tmp;
        tmp = (struct pollfd*)realloc(_fds,sizeof(struct pollfd)*_capactiy*3);
        if(tmp == nullptr)
        {
            perror("malloc fail");
            exit(-1);
        }
        _capactiy*=3;
        _fds = tmp;
    }

    ssize_t size()
    {
        return _size;
    }

    bool isFull()
    {
        return _size == _capactiy;
    }

    void closePollFd(struct pollfd &fd)
    {
        int index = &fd - _fds; // 计算下标
        close(fd.fd);

        // 把最后一个有效元素搬过来，避免空洞
        if (index != _size - 1)
        {
            _fds[index] = _fds[_size - 1];
        }

        _size--;
    }

    void addConnect(int fd)
    {
        _size++;
        if(isFull())
        {
            reSize();
        }
        for (int i = 0; i < _size; i++)
        {
            if (_fds[i].fd == -1)
            {
                _fds[i].fd = fd;
                _fds[i].events = POLLIN | POLLRDHUP;
                break;
            }
        }
    }

protected:
    std::shared_ptr<TcpSocket> _listenSocket;
    InetAddr _host;
    ssize_t _size;
    ssize_t _capactiy;
    struct pollfd *_fds;
    int _timeout;
};

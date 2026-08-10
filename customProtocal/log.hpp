#pragma once
#include <cstdio>
#include <unistd.h>
#include <string>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <ctime>

#define INFO 1
#define DEBUG 2
#define WARN 3
#define ERROR 4
#define FATAL 5
#define DEFAULT_PATH "./log"

const static std::string INFO_LOG = "Info.txt";
const static std::string DEBUG_LOG = "Debug.txt";
const static std::string WARN_LOG = "Warn.txt";
const static std::string ERROR_LOG = "Error.txt";
const static std::string FATAL_LOG = "Fatal.txt";
const static std::string COMPLETE_LOG = "Log.txt";
const static std::string SCREEN_LOG = "Screen";

const static Log LOG;

#define BUFFER_SIZE 1024
#define DATA_SIZE 125
typedef int logLevel;
typedef std::string fileName;
class Log
{
public:
    void operator()(logLevel level, const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);
        vScreenPrint(level, format, ap); // ✅ 正确传递 va_list
        va_end(ap);
    }

    void vScreenPrint(logLevel level, const char *format, va_list ap)
    {
        char buffer[BUFFER_SIZE];
        vsnprintf(buffer, sizeof(buffer), format, ap);
        logWrite(SCREEN_LOG, level, buffer);
    }
    Log(const std::string &path = DEFAULT_PATH) : _path(path)
    {
        if (path == SCREEN_LOG)
        {
        }
        else
        {
            int ret = mkdir(_path.c_str(), 0755);
            if (ret == -1)
            {
                printf("文件夹已经存在!\n");
            }
        }
    }
    ~Log()
    {
    }

    // 直接将日志打印到屏幕
    void screenPrint(logLevel level, const char *format, ...)
    {
        char buffer[BUFFER_SIZE];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, sizeof(buffer), format, ap);
        va_end(ap);
        logWrite(SCREEN_LOG, level, buffer);
    }

    // 这个函数是将日志信息分类存档案
    void sortFilePrint(logLevel level, const char *format, ...)
    {
        char buffer[BUFFER_SIZE];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, sizeof(buffer), format, ap);
        va_end(ap);
        logWrite(levelToFile(level), level, buffer);
    }

    // 这个函数指的是将所有的日志信息打印到一个文件

    void oneFilePrint(logLevel level, const char *format, ...)
    {
        char buffer[BUFFER_SIZE];
        va_list ap;
        va_start(ap, format);
        vsnprintf(buffer, sizeof(buffer), format, ap);
        va_end(ap);
        logWrite(COMPLETE_LOG, level, buffer);
    }

    // 这个函数用于统一向某个文件输入

private:
    const std::string _path;

    void logWrite(const fileName &file, const logLevel &level, const std::string &completeLog)
    {
        char date[DATA_SIZE];
        date[0] = 0;
        time_t t = time(nullptr);
        struct tm *ctime = localtime(&t);
        snprintf(date, sizeof(date), "[%d-%d-%d %d:%d:%d]", ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
                 ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
        char buffer[BUFFER_SIZE];
        buffer[0] = 0;
        snprintf(buffer, sizeof(buffer), "%s [%s]:%s\n", date, levelToString(level).c_str(), completeLog.c_str());
        if (file == SCREEN_LOG)
        {
            ssize_t sz = write(1, buffer, strlen(buffer));
            if (sz == -1)
            {
                perror("write fail!");
                return;
            }
        }
        else
        {

            const std::string pathName = _path + "/" + file;
            int writeFd = open(pathName.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
            if (writeFd == -1)
            {
                perror("open fail!");
            }
            ssize_t sz = write(writeFd, buffer, strlen(buffer));
            if (sz == -1)
            {
                perror("write fail!");
                return;
            }
            close(writeFd);
        }
    }

    const std::string &levelToFile(const logLevel &level)
    {
        switch (level)
        {
        case INFO:
            return INFO_LOG;
        case DEBUG:
            return DEBUG_LOG;
        case WARN:
            return WARN_LOG;
        case ERROR:
            return ERROR_LOG;
        case FATAL:
            return FATAL_LOG;
        default:
            return COMPLETE_LOG;
        }
    }
    const std::string levelToString(const logLevel &level)
    {

        // case INFO:
        // return "INFO";这种写法是不对的，字面常量在函数中，这样会导致返回空引用const std::string& 将&给删除即可

        switch (level)
        {
        case INFO:
            return "INFO";
        case DEBUG:
            return "DEBUG";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "NONE";
        }
    }
};

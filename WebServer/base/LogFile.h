// @Author Gao En Dong
// @Email 18941649210@163.com
#pragma once
#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"
#include <memory>
#include <string>

// TODO 提供自动归档功能
class LogFile : noncopyable
{
public:
    // 每被append flushEveryN次，flush一下，会往文件写，只不过，文件也是带缓冲区的
    LogFile(const std::string& basename, int flushEveryN = 1024);
    ~LogFile();

    void append(const char* logline, int len);//添加到缓存区
    void flush();//清空缓冲区
    bool rollFile();

private:
    void append_unlocked(const char* logline, int len);//不加锁的方式添加

    const std::string basename_;
    const int flushEveryN_;//是否写入日志文件

    int count_;
    std::unique_ptr<MutexLock> mutex_;
    std::unique_ptr<AppendFile> file_;
};
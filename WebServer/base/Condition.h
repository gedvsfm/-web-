// @Author Gao En Dong
// @Email 18941649210@163.com
#pragma once
#include "noncopyable.h"
#include "MutexLock.h"
#include <pthread.h>
#include <errno.h>
#include <cstdint>
#include <time.h>

class Condition: noncopyable
{
public:
    explicit Condition(MutexLock &mutex):
        mutex_(mutex)
    {
        pthread_cond_init(&pcond_, NULL);
    }
    ~Condition()
    {
        pthread_cond_destroy(&pcond_);
    }
    void wait()
    {
        pthread_cond_wait(&pcond_, mutex_.get());
    }    
	//如果超时返回true，否则返回false
    bool waitForSeconds(int seconds)
    {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&pcond_, mutex_.get(), &abstime);
    }
	void notify()
    {
        pthread_cond_signal(&pcond_);
    }
    void notifyAll()
    {
        pthread_cond_broadcast(&pcond_);
    }
private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};
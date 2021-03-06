// @Author Gao En Dong
// @Email 18941649210@163.com
#include "EventLoopThreadPool.h"

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, int numThreads)
:   baseLoop_(baseLoop),
    started_(false),
    numThreads_(numThreads),
    next_(0)
{
    if (numThreads_ <= 0)
    {
        LOG << "numThreads_ <= 0";
        abort();
    }
}
//mainreactor运行
void EventLoopThreadPool::start()
{
    baseLoop_->assertInLoopThread();
    started_ = true;
    for (int i = 0; i < numThreads_; ++i)
    {
        std::shared_ptr<EventLoopThread> t(new EventLoopThread());
        threads_.push_back(t);
        loops_.push_back(t->startLoop());
    }
}
//获取下一个evenloop
EventLoop *EventLoopThreadPool::getNextLoop()
{
    baseLoop_->assertInLoopThread();
    assert(started_);
    EventLoop *loop = baseLoop_;
    if (!loops_.empty())
    {
        loop = loops_[next_];
        next_ = (next_ + 1) % numThreads_;
    }
    return loop;
}
// @Author Gao En Dong
// @Email 18941649210@163.com

#pragma once
#include "CountDownLatch.h"
#include "noncopyable.h"
#include "Channel.h"
#include "Thread.h"
#include <pthread.h>
#include <functional>
#include <memory>
#include <vector>

class ThreadPool : noncopyable
{
 public:
  typedef std::function<void(std::shared_ptr<void>)> Task;
  explicit ThreadPool(const string& name = string());
  ~ThreadPool();

  void start(int numThreads);
  void stop();

  void run(const Task& f);

 private:
  void runInThread();
  Task take();
  MutexLock mutex_;
  Condition cond_;
  string name_;
  std::vector<Thread> threads_;
  std::deque<ThreadPoolTask> queue_;
  bool running_;
};

//
// Created by ltc on 2021/3/7.
//

#ifndef MYPTHREAD_THREADPOOL_H
#define MYPTHREAD_THREADPOOL_H

#include "Thread.h"
#include "Condition.h"
#include <atomic>
#include <queue>
#include <vector>

using std::queue;
using std::vector;

class ThreadPool {
public:
    ThreadPool(int threadNum, int queueSize);
    void addTask(void (*task)(void*), void* arg);
    void close();
    void join();
    int getRunningNum() const;
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    void operator=(const ThreadPool&) = delete;
    void operator=(ThreadPool&&) = delete;
    ~ThreadPool();
private:
    void run();
    static void cleanHandler(void* arg);
    static void* taskRoutine(void* arg);
private:
    struct TaskNode{
        void (*task)(void *);
        void* arg;
        TaskNode(void (*task)(void *), void* arg):task(task), arg(arg){};
    };
    Mutex mutex;
    std::atomic<int> runningNum;
    const int queueSize;
    volatile int shutdown;
    Condition condition;
    vector<Thread> threadPool;
    queue<TaskNode> taskQueue;
};


#endif //MYPTHREAD_THREADPOOL_H

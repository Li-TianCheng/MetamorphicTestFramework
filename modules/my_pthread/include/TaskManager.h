//
// Created by ltc on 2021/5/8.
//

#ifndef MYPTHREAD_TASKMANAGER_H
#define MYPTHREAD_TASKMANAGER_H

#include "ThreadPool.h"

#define THREAD_NUM 12
#define QUEUE_SIZE 100

class TaskManager{
public:
    static void addTask(void (*task)(void*), void* arg);
    static void join();
    static void close();
    TaskManager() = delete;
    TaskManager(const TaskManager&) = delete;
    TaskManager(TaskManager&&) = delete;
    void operator=(const TaskManager&) = delete;
    void operator=(TaskManager&&) = delete;
private:
    static ThreadPool& getInstance();
};


#endif //MYPTHREAD_TASKMANAGER_H

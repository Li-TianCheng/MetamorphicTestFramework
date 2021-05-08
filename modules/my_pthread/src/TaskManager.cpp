//
// Created by ltc on 2021/5/8.
//

#include "TaskManager.h"

void TaskManager::addTask(void (*task)(void*), void* arg) {
    getInstance().addTask(task, arg);
}

void TaskManager::join() {
    getInstance().join();
}

void TaskManager::close() {
    getInstance().close();
}

ThreadPool& TaskManager::getInstance() {
    static ThreadPool threadPool(THREAD_NUM, QUEUE_SIZE);
    return threadPool;
}

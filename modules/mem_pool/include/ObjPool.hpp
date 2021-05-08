//
// Created by ltc on 2021/5/8.
//

#ifndef MEMPOOL_OBJPOOL_HPP
#define MEMPOOL_OBJPOOL_HPP

#include "MemPool.h"

#define CHUNK_SIZE 100

class ObjPool{
public:
    template<typename T, typename... Args>
    static T* allocate(Args... args);

    template<typename T>
    static void deallocate(T* ptr);

    ObjPool() = delete;
    ObjPool(const ObjPool&) = delete;
    ObjPool(TaskManager&&) = delete;
    void operator=(const ObjPool&) = delete;
    void operator=(ObjPool&&) = delete;
private:
    static MemPool& getInstance();
};

template<typename T, typename... Args> inline
T* ObjPool::allocate(Args... args) {
    void* ptr = getInstance().allocate(sizeof(T));
    return new(ptr) T(args...);
}

template<typename T> inline
void ObjPool::deallocate(T* ptr) {
    ptr->~T();
    getInstance().deallocate(ptr, sizeof(T));
}

MemPool& ObjPool::getInstance() {
    static MemPool memPool(CHUNK_SIZE);
    return memPool;
}

#endif //MEMPOOL_OBJPOOL_HPP

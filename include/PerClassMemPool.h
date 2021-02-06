//
// Created by 76484 on 2021/1/28.
//

#ifndef PERCLASSMEMPOOL_H
#define PERCLASSMEMPOOL_H

#include <cstddef>

class PerClassMemPool {
public:
    explicit PerClassMemPool(int chunk);
    void* allocate(size_t size);
    void deallocate(void* ptr);
private:
    struct obj{
        obj* next;
    };
    obj* freePointer = nullptr;
    const int chunk;
};


#endif //PERCLASSMEMPOOL_H

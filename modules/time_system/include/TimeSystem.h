//
// Created by ltc on 2021/5/14.
//

#ifndef TIMESYSTEM_TIMESYSTEM_H
#define TIMESYSTEM_TIMESYSTEM_H

#include "TimeWheel.h"
#include "my_pthread/include/Thread.h"

class TimeSystem{
public:
    static void init();
    static void close();
    static void addEvent(int eventType, Time* arg, EventSystem* ptr);
    TimeSystem(const TimeSystem&) = delete;
    TimeSystem(TimeSystem&&) = delete;
    TimeSystem& operator=(const TimeSystem&) = delete;
    TimeSystem& operator=(TimeSystem&&) = delete;
private:
    TimeSystem() = default;
    static TimeWheel& getTimeWheel();
    static Thread& getThread();
    static void* handle(void*);
};




#endif //TIMESYSTEM_TIMESYSTEM_H

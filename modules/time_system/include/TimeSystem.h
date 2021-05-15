//
// Created by ltc on 2021/5/14.
//

#ifndef METAMORPHICTESTFRAMEWORK_TIMESYSTEM_H
#define METAMORPHICTESTFRAMEWORK_TIMESYSTEM_H

#include "TimeWheel.h"
#include "my_pthread/include/Thread.h"

class TimeSystem{
public:
    static void init();
    static void close();
    static void addEvent(const string& name, Time* arg, EventSystem* ptr);
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




#endif //METAMORPHICTESTFRAMEWORK_TIMESYSTEM_H

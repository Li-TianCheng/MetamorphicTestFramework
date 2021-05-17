//
// Created by ltc on 2021/5/14.
//

#include "TimeSystem.h"

void TimeSystem::init() {
    getTimeWheel();
    getThread().run(handle, nullptr);
}

void *TimeSystem::handle(void *) {
    getTimeWheel().cycle();
}

void TimeSystem::close() {
    Event* e = ObjPool::allocate<Event>(EventEndCycle, nullptr, nullptr);
    getTimeWheel().addEvent(e);
    getThread().join();
}

void TimeSystem::addEvent(int eventType, Time *arg, EventSystem *ptr) {
    arg->tPtr = &getTimeWheel();
    Event* e = ObjPool::allocate<Event>(eventType, arg, ptr);
    getTimeWheel().addEvent(e);
}

TimeWheel &TimeSystem::getTimeWheel() {
    static TimeWheel timeWheel;
    return timeWheel;
}

Thread &TimeSystem::getThread() {
    static Thread thread;
    return thread;
}


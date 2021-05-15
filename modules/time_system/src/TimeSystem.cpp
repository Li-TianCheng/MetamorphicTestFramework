//
// Created by ltc on 2021/5/14.
//

#include "TimeSystem.h"

void TimeSystem::init() {
    getThread().run(handle, nullptr);
}

void *TimeSystem::handle(void *) {
    getTimeWheel().cycle();
    return nullptr;
}

void TimeSystem::close() {
    getTimeWheel().addEvent("end_event", nullptr, nullptr);
    getThread().join();
}

void TimeSystem::addEvent(const string& name, Time *arg, EventSystem *ptr) {
    arg->tPtr = &getTimeWheel();
    getTimeWheel().addEvent(name, arg, ptr);
}

TimeWheel &TimeSystem::getTimeWheel() {
    static TimeWheel timeWheel;
    return timeWheel;
}

Thread &TimeSystem::getThread() {
    static Thread thread;
    return thread;
}


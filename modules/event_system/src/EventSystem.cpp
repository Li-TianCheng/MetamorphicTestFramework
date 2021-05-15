//
// Created by ltc on 2021/5/14.
//

#include "EventSystem.h"

void EventSystem::registerEvent(const string& name, void (*handleEvent)(void*, EventSystem*)) {
    map[name] = handleEvent;
}

void EventSystem::unregisterEvent(const string& name) {
    map.erase(name);
}

void EventSystem::addEvent(const string& name, void* arg, EventSystem* ptr) {
    mutex.lock();
    eventQueue.push(Event(name, arg, ptr));
    mutex.unlock();
}

void EventSystem::doEvent(Event& e) {
    if (map.find(e.name) != map.end()){
        map[e.name](e.arg, e.ptr);
    }
}

Event EventSystem::getEvent() {
    if (!mutex.tryLock()){
        return Event("null", nullptr, nullptr);
    }
    if (eventQueue.empty()) {
        mutex.unlock();
        return Event("null", nullptr, nullptr);
    }
    Event e = eventQueue.front();
    eventQueue.pop();
    mutex.unlock();
    return e;
}


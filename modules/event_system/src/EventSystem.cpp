//
// Created by ltc on 2021/5/14.
//

#include "EventSystem.h"

void EventSystem::registerEvent(int eventType, void (*handleEvent)(Event*)) {
    map[eventType] = handleEvent;
}

void EventSystem::unregisterEvent(int eventType) {
    map.erase(eventType);
}

void EventSystem::addEvent(Event* e) {
    mutex.lock();
    eventQueue.push(e);
    mutex.unlock();
}

void EventSystem::doEvent(Event* e) {
    if (map.find(e->eventType) != map.end()){
        map[e->eventType](e);
    }
    ObjPool::deallocate(e);
}

Event* EventSystem::getEvent() {
    if (!mutex.tryLock()){
        return nullptr;
    }
    if (eventQueue.empty()) {
        mutex.unlock();
        return nullptr;
    }
    Event* e = eventQueue.front();
    eventQueue.pop();
    mutex.unlock();
    return e;
}


//
// Created by ltc on 2021/5/14.
//

#ifndef EVENTSYSTEM_EVENTSYSTEM_H
#define EVENTSYSTEM_EVENTSYSTEM_H

#include <unordered_map>
#include <string>
#include <queue>
#include "mem_pool/include/ObjPool.hpp"
#include "my_pthread/include/Mutex.h"
#include "my_pthread/include/Thread.h"
#include "EventKey.h"

using std::unordered_map;
using std::string;
using std::queue;

class EventSystem;

struct Event{
    int eventType;
    EventSystem* ptr;
    void* arg;
    Event(int eventType, void* arg, EventSystem* ptr) : eventType(eventType), arg(arg), ptr(ptr){};
};

class EventSystem {
public:
    EventSystem() = default;
    void registerEvent(int eventType, void (*handleEvent)(Event*));
    void unregisterEvent(int eventType);
    void addEvent(Event* e);
    void doEvent(Event* e);
    Event* getEvent();
    virtual void cycle() = 0;
    EventSystem(const EventSystem &) = delete;
    EventSystem(EventSystem&&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;
    EventSystem& operator=(EventSystem&&) = delete;
private:
    unordered_map<int, void(*)(Event*)> map;
    queue<Event*> eventQueue;
    Mutex mutex;
    vector<Thread> worker;
};


#endif //EVENTSYSTEM_EVENTSYSTEM_H

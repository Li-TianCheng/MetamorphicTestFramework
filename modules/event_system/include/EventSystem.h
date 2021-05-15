//
// Created by ltc on 2021/5/14.
//

#ifndef EVENTSYSTEM_EVENTSYSTEM_H
#define EVENTSYSTEM_EVENTSYSTEM_H

#include <unordered_map>
#include <string>
#include <queue>

#include "my_pthread/include/Mutex.h"

using std::unordered_map;
using std::string;
using std::queue;

class EventSystem;

struct Event{
    string name;
    EventSystem* ptr;
    void* arg;
    Event(string name, void* arg, EventSystem* ptr) : name(std::move(name)), arg(arg), ptr(ptr){};
    bool operator==(const string& n) const{
        return name == n;
    }
    bool operator!=(const string& n) const{
        return name != n;
    }
};

class EventSystem {
public:
    EventSystem() = default;
    void registerEvent(const string& name, void (*handleEvent)(void*, EventSystem*));
    void unregisterEvent(const string& name);
    void addEvent(const string& name, void* arg, EventSystem* ptr);
    void doEvent(Event& e);
    Event getEvent();
    virtual void cycle() = 0;
    EventSystem(const EventSystem &) = delete;
    EventSystem(EventSystem&&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;
    EventSystem& operator=(EventSystem&&) = delete;
private:
    unordered_map<string, void(*)(void*, EventSystem*)> map;
    queue<Event> eventQueue;
    Mutex mutex;
};


#endif //EVENTSYSTEM_EVENTSYSTEM_H

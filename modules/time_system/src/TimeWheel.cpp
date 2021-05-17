//
// Created by ltc on 2021/5/14.
//
#include <iostream>
#include "time_system/include/TimeWheel.h"

TimeWheel::TimeWheel(): millisecond(1000), second(60), minute(60), hour(24),
                        msIter(0), sIter(0), mIter(0), hIter(0) {
    init();
}

void TimeWheel::cycle() {
    while (true) {
        Event* e = getEvent();
        if (e != nullptr){
            if (e->eventType == EventEndCycle){
                break;
            }
            doEvent(e);
        }
        check();
    }
}

void TimeWheel::init() {
    registerEvent(EventTimer, handleTimerEvent);
    registerEvent(EventTicker, handleTickerEvent);
    registerEvent(EventTimerTimeOut, handleTimerTimeOut);
    registerEvent(EventTickerTimeOut, handleTickerTimeOut);
    registerEvent(EventEndCycle, nullptr);
}

void TimeWheel::handleTimerEvent(Event* e) {
    if (e->arg == nullptr){
        return;
    }
    Time* input = (Time*)e->arg;
    TimeWheel* t = input->tPtr;
    Event* _e = ObjPool::allocate<Event>(EventTimerTimeOut, input, e->ptr);
    if (input->h != 0){
        t->hour[(t->hIter+input->h)%24].push(_e);
        return;
    }
    if (input->m != 0){
        t->minute[(t->mIter+input->m)%60].push(_e);
        return;
    }
    if (input->s != 0){
        t->second[(t->sIter+input->s)%60].push(_e);
        return;
    }
    if (input->ms != 0){
        t->millisecond[(t->msIter+input->ms)%1000].push(_e);
        return;
    }
    ObjPool::deallocate(_e);
}

void TimeWheel::handleTickerEvent(Event* e) {
    if (e->arg == nullptr){
        return;
    }
    Time* input = (Time*)e->arg;
    TimeWheel* t = input->tPtr;
    Event* _e = ObjPool::allocate<Event>(EventTickerTimeOut, input, e->ptr);
    if (input->h != 0){
        t->hour[(t->hIter+input->h)%24].push(_e);
        return;
    }
    if (input->m != 0){
        t->minute[(t->mIter+input->m)%60].push(_e);
        return;
    }
    if (input->s != 0){
        t->second[(t->sIter+input->s)%60].push(_e);
        return;
    }
    if (input->ms != 0){
        t->millisecond[(t->msIter+input->ms)%1000].push(_e);
        return;
    }
    ObjPool::deallocate(_e);
}

void TimeWheel::check() {
    if (getCurrentTime() - currentTime < 1000){
        return;
    }
    currentTime = getCurrentTime();
    while (!millisecond[msIter].empty()){
        Event* e = millisecond[msIter].front();
        doEvent(e);
        millisecond[msIter].pop();
    }
    msIter = (msIter+1) % 1000;
    if (msIter == 0){
        sIter = (sIter+1) % 60;
        while(!second[sIter].empty()){
            Event* e = second[sIter].front();
            millisecond[((Time*)e->arg)->ms].push(e);
            second[sIter].pop();
        }
    }
    if (sIter == 0){
        mIter = (mIter+1) % 60;
        while(!minute[mIter].empty()){
            Event* e = minute[mIter].front();
            second[((Time*)e->arg)->s].push(e);
            minute[mIter].pop();
        }
    }
    if (mIter == 0){
        hIter = (hIter+1) % 24;
        while(!hour[hIter].empty()){
            Event* e = hour[hIter].front();
            minute[((Time*)e->arg)->m].push(e);
            hour[hIter].pop();
        }
    }
}

u_long TimeWheel::getCurrentTime() {
    timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1e6+t.tv_usec;
}

void TimeWheel::handleTimerTimeOut(Event *e) {
    if (e->ptr != nullptr){
        Event* _e = ObjPool::allocate<Event>(EventTimerTimeOut, e->ptr, ((Time*)e)->tPtr);
        e->ptr->addEvent(_e);
    }
}

void TimeWheel::handleTickerTimeOut(Event *e) {
    if (e->ptr != nullptr){
        Event* _e = ObjPool::allocate<Event>(EventTickerTimeOut, e->ptr, ((Time*)e)->tPtr);
        e->ptr->addEvent(_e);
    }
    Event* _e = ObjPool::allocate<Event>(EventTicker, e->arg, e->ptr);
    handleTickerEvent(_e);
}


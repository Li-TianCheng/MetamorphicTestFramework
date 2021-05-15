//
// Created by ltc on 2021/5/14.
//

#ifndef METAMORPHICTESTFRAMEWORK_TIMEWHEEL_H
#define METAMORPHICTESTFRAMEWORK_TIMEWHEEL_H

#include <vector>
#include <sys/time.h>
#include "event_system/include/EventSystem.h"

using std::vector;

class TimeWheel: public EventSystem {
public:
    TimeWheel();
    void cycle() override;
private:
    void init();
    void check();
    static u_long getCurrentTime();
    vector<queue<Event>> millisecond;
    vector<queue<Event>> second;
    vector<queue<Event>> minute;
    vector<queue<Event>> hour;
    int msIter;
    int sIter;
    int mIter;
    int hIter;
    u_long currentTime;
    static void handleTimerEvent(void* arg, EventSystem* ptr);
    static void handleTickerEvent(void* arg, EventSystem* ptr);
};

struct Time{
    TimeWheel* tPtr = nullptr;
    int ms;
    int s;
    int m;
    int h;
    Time(int h, int m, int s, int ms): ms(ms), s(s), m(m), h(h){};
};

#endif //METAMORPHICTESTFRAMEWORK_TIMEWHEEL_H

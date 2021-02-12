//
// Created by Heisenberg Liu on 2021/2/12.
//

#ifndef CHANGRATELIMITER_CHANGRATELIMITERIMPL_H
#define CHANGRATELIMITER_CHANGRATELIMITERIMPL_H

#include "../ChangRateLimiter.h"
#include <chrono>
#include <mutex>

class ChangRateLimiterImpl: public ChangRateLimiter {
public:
    ChangRateLimiterImpl(int maxSize,int token,int second);
    void acquire();

    void acquire(int requestNum);

    bool tryAcquire(int timeout);

    bool tryAcquire(int requestNum,int timeout);

private:
    void sync(unsigned long long now);
    std::chrono::microseconds getWaitTime(int requestNum);

    int maxSize;
    int storedPermit;
    unsigned long long nextAvailableTime;
    double tokenPerMillSec;
    std::mutex _mutex;
};

unsigned long long getCurrentMicroSecondTime(){
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

#endif //CHANGRATELIMITER_CHANGRATELIMITERIMPL_H

//
// Created by Heisenberg Liu on 2021/2/12.
//

#include "ChangRateLimiterImpl.h"
#include <chrono>
#include <stdexcept>
#include <thread>
using namespace std;

ChangRateLimiterImpl::ChangRateLimiterImpl(int maxSize, int token, int second) :
        maxSize(maxSize),
        nextAvailableTime(0),
        storedPermit(0),
        tokenPerMillSec(0) {
    if (maxSize<=0){
        throw runtime_error("max size of permit cannot be lower than 0");
    }
    if (token<=0 || second <=0){
        throw runtime_error("error token or second parameters");
    }
    this->tokenPerMillSec = static_cast<double>(token) / static_cast<double>(second)*1000000;
}


void ChangRateLimiterImpl::sync(unsigned long long now) {
    if (now<nextAvailableTime){
        return;
    }
    storedPermit = max(maxSize,(int)((now-nextAvailableTime)/tokenPerMillSec));
}

/**
 * 1.synchronize permit storage
 * 2.check unfinished number
 * 3.update next available time
 * 4.return waiting time
 * @param requestNum
 * @return
 */
std::chrono::microseconds ChangRateLimiterImpl::getWaitTime(int requestNum) {
    auto now = getCurrentMicroSecondTime();
    lock_guard<std::mutex> lock(_mutex);
    sync(now);
    int remainRequest = max(0,requestNum-storedPermit);
    storedPermit = max(0,storedPermit-requestNum);
    unsigned long long waitTime = remainRequest/tokenPerMillSec;
    nextAvailableTime = waitTime+now;
    return chrono::microseconds(waitTime);
}

void ChangRateLimiterImpl::acquire() {
    acquire(1);
}

void ChangRateLimiterImpl::acquire(int requestNum) {
    if (requestNum<=0){
        throw runtime_error("request number cannot be lower than 0");
    }
    chrono::microseconds waitTime = getWaitTime(requestNum);
    this_thread::sleep_for(waitTime);
}

bool ChangRateLimiterImpl::tryAcquire(int timeout) {
    return tryAcquire(1, timeout);
}


bool ChangRateLimiterImpl::tryAcquire(int requestNum, int timeout) {
    if (requestNum<=0){
        throw runtime_error("request number cannot be lower than 0");
    }
    if (timeout<=0){
        throw runtime_error("timeout cannot be lower than 0");
    }
    using namespace chrono;
    unsigned long long currentTime = getCurrentMicroSecondTime();
    if (nextAvailableTime>currentTime+timeout*1000){
        return false;
    }
    acquire(requestNum);
    return true;
}


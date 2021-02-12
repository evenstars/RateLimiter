//
// Created by Heisenberg Liu on 2021/2/12.
//

#ifndef CHANGRATELIMITER_CHANGRATELIMITER_H
#define CHANGRATELIMITER_CHANGRATELIMITER_H

/**
 * Rate limiter based on token bucket algorithm.
 */
class ChangRateLimiter{
public:
    virtual void acquire()=0;

    virtual void acquire(int requestNum)=0;

    /**
     * time out is for millisecond.
     * @param timeout
     * @return
     */
    virtual bool tryAcquire(int timeout)=0;

    virtual bool tryAcquire(int requestNum,int timeout)=0;
};
#endif //CHANGRATELIMITER_CHANGRATELIMITER_H

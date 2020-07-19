//
// Created by 阳坤 on 2020-07-03.
//

#ifndef RTMPPUSH_ITHREAD_H
#define RTMPPUSH_ITHREAD_H


#include <pthread.h>


/**
 * 线程控制的基类
 */
class IThread {

public:

    /**
     * 线程 ID
     */
    pthread_t pId;

public:
    virtual void start();

    virtual void stop();

    virtual void main() = 0;


};


#endif //RTMPPUSH_ITHREAD_H

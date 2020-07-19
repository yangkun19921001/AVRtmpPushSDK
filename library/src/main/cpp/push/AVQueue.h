//
// Created by yangw on 2018-9-14.
//

#ifndef AVLIVEPUSHER_QUEUE_H
#define AVLIVEPUSHER_QUEUE_H

#include "queue"
#include "pthread.h"
#include <android/log.h>

extern "C"
{
#include "../librtmp/include/rtmp.h"
};


class AVQueue {

public:
    std::queue<RTMPPacket *> queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;

public:
    AVQueue();
    ~AVQueue();

    int putRtmpPacket(RTMPPacket *packet);

    RTMPPacket* getRtmpPacket();

    void clearQueue();

    void notifyQueue();


};


#endif //AVLIVEPUSHER_QUEUE_H

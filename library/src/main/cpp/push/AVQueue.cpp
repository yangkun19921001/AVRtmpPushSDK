//
// Created by yangw on 2018-9-14.
//

#include "AVQueue.h"

AVQueue::AVQueue() {
    pthread_mutex_init(&mutexPacket, 0);
    pthread_cond_init(&condPacket, 0);
}

AVQueue::~AVQueue() {
    clearQueue();
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);

}

int AVQueue::putRtmpPacket(RTMPPacket *packet) {
    pthread_mutex_lock(&mutexPacket);
    queuePacket.push(packet);
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

RTMPPacket *AVQueue::getRtmpPacket() {
    pthread_mutex_lock(&mutexPacket);

    RTMPPacket *p = 0;
    if(!queuePacket.empty())
    {
        p = queuePacket.front();
        queuePacket.pop();
    } else{
        pthread_cond_wait(&condPacket, &mutexPacket);
    }
    pthread_mutex_unlock(&mutexPacket);
    return p;
}

void AVQueue::clearQueue() {

    pthread_mutex_lock(&mutexPacket);
    while(true)
    {
        if(queuePacket.empty())
        {
            break;
        }
        RTMPPacket *p = queuePacket.front();
        queuePacket.pop();
        RTMPPacket_Free(p);
        p = 0;
    }
    pthread_mutex_unlock(&mutexPacket);

}

void AVQueue::notifyQueue() {

    pthread_mutex_lock(&mutexPacket);
    pthread_cond_signal(&condPacket);
    pthread_mutex_unlock(&mutexPacket);

}

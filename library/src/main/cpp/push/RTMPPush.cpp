//
// Created by 阳坤 on 2020-07-03.
//


#include "RTMPPush.h"

RTMPPush::RTMPPush(const char *url, JavaCallback **javaCallback) {
    //防止 java 传递过来的 jstring 释放。
    char *rtmpUrl = new char[strlen(url) + 1];
    strcpy(rtmpUrl, url);
    this->mRtmpUrl = rtmpUrl;
    this->mQueue = new AVQueue();
    this->mCallback = *javaCallback;
}


RTMPPush::~RTMPPush() {

    LOGE("RTMP ~RTMPPush close success!")

}

void RTMPPush::start() {
    IThread::start();
}

void RTMPPush::stop() {
    isPusher = false;
    int v = 1000;

    if (!mQueue)
        return;
    this->mQueue->notifyQueue();
    this->mQueue->clearQueue();
    free(this->mRtmpUrl);
    mRtmpUrl = 0;
    mQueue = 0;
    //等待片刻
    while (v > 0) {
        v--;
        continue;
    }
}

/**
 * 运行在子线程中
 */
void RTMPPush::main() {
    //开始连接
    onConnecting();

}

void RTMPPush::pushSpsPps(uint8_t *sps, int sps_len, uint8_t *pps, int pps_len) {
    int bodysize = sps_len + pps_len + 16;
    RTMPPacket *packet = static_cast<RTMPPacket *>(malloc(sizeof(RTMPPacket)));
    RTMPPacket_Alloc(packet, bodysize);
    RTMPPacket_Reset(packet);

    char *body = packet->m_body;

    int i = 0;

    body[i++] = 0x17;

    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    body[i++] = 0x01;
    body[i++] = sps[1];
    body[i++] = sps[2];
    body[i++] = sps[3];

    body[i++] = 0xFF;

    body[i++] = 0xE1;
    body[i++] = (sps_len >> 8) & 0xff;
    body[i++] = sps_len & 0xff;
    memcpy(&body[i], sps, sps_len);
    i += sps_len;

    body[i++] = 0x01;
    body[i++] = (pps_len >> 8) & 0xff;
    body[i++] = pps_len & 0xff;
    memcpy(&body[i], pps, pps_len);

    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = bodysize;
    packet->m_nTimeStamp = 0;
    packet->m_hasAbsTimestamp = 0;
    packet->m_nChannel = 0x04;
    packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    packet->m_nInfoField2 = mRtmp->m_stream_id;

    mQueue->putRtmpPacket(packet);

}

void RTMPPush::pushAudioData(uint8_t *data, int data_len, int type) {
    RTMPPacket *packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, data_len);
    RTMPPacket_Reset(packet);
    packet->m_nChannel = 0x05; //音频
    memcpy(packet->m_body, data, data_len);
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_hasAbsTimestamp = FALSE;
    packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet->m_nBodySize = data_len;
    packet->m_nTimeStamp = RTMP_GetTime() - mStartTime;


    /* int bodysize = data_len + 2;
     RTMPPacket *packet = static_cast<RTMPPacket *>(malloc(sizeof(RTMPPacket)));
     RTMPPacket_Alloc(packet, bodysize);
     RTMPPacket_Reset(packet);
     char *body = packet->m_body;
     body[0] = 0xAF;
     body[1] = 0x01;
     memcpy(&body[2], data, data_len);

     packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
     packet->m_nBodySize = bodysize;
     packet->m_nTimeStamp = RTMP_GetTime() - mStartTime;
     packet->m_hasAbsTimestamp = 0;
     packet->m_nChannel = 0x05;
     packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
     packet->m_nInfoField2 = mRtmp->m_stream_id;*/
    mQueue->putRtmpPacket(packet);

}

void RTMPPush::pushVideoData(uint8_t *data, int data_len, int type) {
    RTMPPacket *packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, data_len);
    RTMPPacket_Reset(packet);

    packet->m_nChannel = 0x04; //视频

    if (type == 5) {
        LOGE("视频关键帧");
    }

    memcpy(packet->m_body, data, data_len);
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_hasAbsTimestamp = FALSE;
    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = data_len;
    packet->m_nTimeStamp = RTMP_GetTime() - mStartTime;



/*
    int bodysize = data_len + 9;
    RTMPPacket *packet = static_cast<RTMPPacket *>(malloc(sizeof(RTMPPacket)));
    RTMPPacket_Alloc(packet, bodysize);
    RTMPPacket_Reset(packet);

    char *body = packet->m_body;
    int i = 0;

    if (type) {
        body[i++] = 0x17;
    } else {
        body[i++] = 0x27;
    }

    body[i++] = 0x01;
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    body[i++] = (data_len >> 24) & 0xff;
    body[i++] = (data_len >> 16) & 0xff;
    body[i++] = (data_len >> 8) & 0xff;
    body[i++] = data_len & 0xff;
    memcpy(&body[i], data, data_len);

    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = bodysize;
    packet->m_nTimeStamp = RTMP_GetTime() - mStartTime;
    packet->m_hasAbsTimestamp = 0;
    packet->m_nChannel = 0x04;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = mRtmp->m_stream_id;*/

    mQueue->putRtmpPacket(packet);

}

void RTMPPush::onConnecting() {

    //开始连接
    if (mCallback) {
        mCallback->onConnecting(THREAD_CHILD);
    }

    //如果已经存在旧的连接那么先释放
    if (mRtmp) {
        relase();
    }

    //拿到 RTMP 实例
    this->mRtmp = RTMP_Alloc();
    if (!mRtmp) {
        if (mCallback) {
            mCallback->onConnectFail(RTMP_INIT_ERROR);
        }
        relase();
        return;
    }

    //rtmp init
    RTMP_Init(mRtmp);
    //设置地址
    int ret = RTMP_SetupURL(mRtmp, mRtmpUrl);
    if (!ret && mCallback) {
        mCallback->onConnectFail(RTMP_SET_URL_ERROR);
        relase();
        return;
    }

    //设置超时时间
    mRtmp->Link.timeout = 5;
    RTMP_EnableWrite(mRtmp);
    //开始链接
    ret = RTMP_Connect(mRtmp, 0);
    if (!ret) {
        if (mCallback) {
            mCallback->onConnectFail(RTMP_CONNECT_ERROR);
        }
        relase();
        return;
    }

    //seek 到某一处
    ret = RTMP_ConnectStream(mRtmp, 0);
    if (!ret) {
        if (mCallback) {
            mCallback->onConnectFail(RTMP_CONNECT_ERROR);
        }
        relase();
        return;
    }
    //记录一个开始时间
    mStartTime = RTMP_GetTime();

    if (mCallback) {
        mCallback->onConnectSuccess();
    }

    //可以开始推流
    this->isPusher = 1;

    while (true) {
        if (!isPusher || !mQueue) {
            relase();
//            if (mCallback) {
//                mCallback->onClose(THREAD_CHILD);
//            }
            break;
        }
        RTMPPacket *packet = 0;
        packet = mQueue->getRtmpPacket();
        if (packet != NULL) {
            packet->m_nInfoField2 = mRtmp->m_stream_id;
            int result = RTMP_SendPacket(mRtmp, packet, 1);
            if (!result) {
                LOGE("RTMP_SendPacket result is %d", result);
            }
            RTMPPacket_Free(packet);
            free(packet);
            packet = NULL;
        }
    }

    LOGE("RTMP close success!")

}

void RTMPPush::relase() {
    if (!mRtmp)
        return;
    RTMP_Close(mRtmp);
    RTMP_Free(mRtmp);
    mRtmp = 0;
}

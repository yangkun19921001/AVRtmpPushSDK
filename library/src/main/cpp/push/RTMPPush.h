//
// Created by 阳坤 on 2020-07-03.
//

#ifndef RTMPPUSH_RTMPPUSH_H
#define RTMPPUSH_RTMPPUSH_H


#include <IThread.h>
#include <AVQueue.h>
#include <JavaCallback.h>

#include <android/log.h>

#define TAG "AVRtmpPush"

#include <malloc.h>
#include <IPush.h>


#define LOG_SHOW true

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG,TAG,FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,TAG,FORMAT,##__VA_ARGS__);

class RTMPPush :public IPush{

public:
    /**
     * rtmp 推流实例
     */
    RTMP *mRtmp = 0;

    /**
     * 推流链接
     */
    char *mRtmpUrl = 0;

    /**
     * rtmp packet queue
     */
    AVQueue *mQueue = 0;

    /**
     * 回调给 Java
     */
    JavaCallback *mCallback = 0;

    /**
     * 是否推流
     */
    int isPusher = 0;

    /**
     * 推流时间
     */
    long mStartTime = 0;

public:
    RTMPPush(const char *url, JavaCallback **javaCallback);

    ~RTMPPush();


    /**
     * 开启线程
     */
    virtual void start();

    /**
     * 停止推流
     */
    virtual void stop();

    /**
     * 子线程入口
     */
    virtual void main();


    virtual void pushSpsPps(uint8_t *sps, int sps_len, uint8_t *pps, int pps_len);

    virtual void pushAudioData(uint8_t *audio, int len,int type);

    virtual  void pushVideoData(uint8_t *video, int len, int type);

    void onConnecting();

    void relase();
};


#endif //RTMPPUSH_RTMPPUSH_H

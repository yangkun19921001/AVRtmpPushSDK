//
// Created by 阳坤 on 2020-07-03.
//

#ifndef RTMPPUSH_IPUSH_H
#define RTMPPUSH_IPUSH_H


#include "IThread.h"

class IPush : public IThread {

public:
    /**
 * 开启线程
 */
    virtual void start() = 0;

    /**
     * 停止推流
     */
    virtual void stop() = 0;

    /**
     * 子线程入口
     */
    virtual void main() = 0;

    /**
     * 推送视频第一帧 SPS PPS 数据
     * @param sps
     * @param sps_len
     * @param pps
     * @param pps_len
     */
    virtual void pushSpsPps(uint8_t *sps, int sps_len, uint8_t *pps, int pps_len) = 0;

    /**
     * 推送音频数据
     * @param audio
     * @param len
     */
    virtual void pushAudioData(uint8_t *audio, int len,int type) = 0;

    /**
     * 推送视频数据
     * @param video
     * @param len
     * @param keyframe
     */
    virtual void pushVideoData(uint8_t *video, int len, int type) = 0;
};


#endif //RTMPPUSH_IPUSH_H

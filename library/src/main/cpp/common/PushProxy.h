//
// Created by 阳坤 on 2020-07-17.
//

#ifndef RTMPPUSH_PUSHPROXY_H
#define RTMPPUSH_PUSHPROXY_H

#include "../push/RTMPPush.h"
#include "IPush.h"

class PushProxy {

private:

    RTMPPush *rtmpPush = 0;
    /**
 * 拿到推流的实体类
 * @return
 */
    IPush *getPushEngine();


public:
    PushProxy();

    static PushProxy * getInstance();




    void init(const char*url,JavaCallback ** javaCallback);

    const char *url = 0;
    JavaCallback *javaCallback = 0;



    /**
* 开启线程
*/
     void start() ;

    /**
     * 停止推流
     */
     void stop() ;



    /**
     * 推送视频第一帧 SPS PPS 数据
     * @param sps
     * @param sps_len
     * @param pps
     * @param pps_len
     */
     void pushSpsPps(uint8_t *sps, int sps_len, uint8_t *pps, int pps_len) ;

    /**
     * 推送音频数据
     * @param audio
     * @param len
     */
     void pushAudioData(uint8_t *audio, int len,int type) ;

    /**
     * 推送视频数据
     * @param video
     * @param len
     * @param keyframe
     */
     void pushVideoData(uint8_t *video, int len, int keyframe) ;

};


#endif //RTMPPUSH_PUSHPROXY_H

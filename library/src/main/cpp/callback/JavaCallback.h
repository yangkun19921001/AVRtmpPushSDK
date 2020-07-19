//
// Created by 阳坤 on 2020-07-17.
//

#ifndef RTMPPUSH_JAVACALLBACK_H
#define RTMPPUSH_JAVACALLBACK_H

#define THREAD_MAIN 1
#define THREAD_CHILD 2


//rtmp 初始化失败
#define RTMP_INIT_ERROR  -9
//设置 rtmp url 失败
#define RTMP_SET_URL_ERROR  -10
//连接服务器失败
#define RTMP_CONNECT_ERROR  -11
//RTMP 关闭成功
#define RTMP_CLOSE  -12



#include <jni.h>

class JavaCallback {

public:
    JNIEnv *jniEnv = 0;
    JavaVM *javaVM = 0;
    jobject jobject1;

    jmethodID jmid_connecting;
    jmethodID jmid_success;
    jmethodID jmid_close;
    jmethodID jmid_fail;

public:
    JavaCallback(JavaVM *vm, JNIEnv *env, jobject obj);

    ~JavaCallback();

    /**
     * 开始链接
     */
    void onConnecting(int threadType);

    /**
     * 链接成功
     */
    void onConnectSuccess();

    /**
     * 链接失败
     */
    void onConnectFail(int errorCode);
    void onClose(int errorCode);

};


#endif //RTMPPUSH_JAVACALLBACK_H

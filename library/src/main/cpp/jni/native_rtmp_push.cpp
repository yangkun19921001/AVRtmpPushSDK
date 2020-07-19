//
// Created by 阳坤 on 2020-07-03.
//
#include <jni.h>
#include <PushProxy.h>


#define NATIVE_PUSH "com/devyk/av/rtmp/library/stream/sender/rtmp/RtmpSender"

JavaVM *javaVM = 0;

static void Android_JNI_RtmpConnect(JNIEnv *jniEnv, jobject jobject1, jstring url) {

    const char *rtmpUrl = (char *) jniEnv->GetStringUTFChars(url, 0);

    JavaCallback *javaCallback = new JavaCallback(javaVM, jniEnv, jobject1);
    PushProxy::getInstance()->init(rtmpUrl, &javaCallback);
    PushProxy::getInstance()->start();
    jniEnv->ReleaseStringUTFChars(url, rtmpUrl);


}

static void Android_JNI_RtmpClose(JNIEnv *jniEnv, jobject jobject1) {
    PushProxy::getInstance()->stop();

}

static void Android_JNI_pushAudio(JNIEnv *jniEnv, jobject jobject1, jbyteArray audio, jint size,jint type) {

    jbyte *audioData = jniEnv->GetByteArrayElements(audio, 0);

    PushProxy::getInstance()->pushAudioData(reinterpret_cast<uint8_t *>(audioData), size,type);

    jniEnv->ReleaseByteArrayElements(audio, audioData, 0);


}

static void Android_JNI_pushVideo(JNIEnv *jniEnv, jobject jobject1, jbyteArray video, jint size, jint type) {
    jbyte *videoData = jniEnv->GetByteArrayElements(video, 0);
    PushProxy::getInstance()->pushVideoData(reinterpret_cast<uint8_t *>(videoData), size, type);
    jniEnv->ReleaseByteArrayElements(video, videoData, 0);
}

static void
Android_JNI_pushSpsPps(JNIEnv *jniEnv, jobject jobject1, jbyteArray sps, jint spsSize, jbyteArray pps, jint ppsSize) {
    jbyte *spsData = jniEnv->GetByteArrayElements(sps, 0);
    jbyte *ppsData = jniEnv->GetByteArrayElements(pps, 0);
    PushProxy::getInstance()->pushSpsPps(reinterpret_cast<uint8_t *>(spsData), spsSize,
                                                          reinterpret_cast<uint8_t *>(ppsData), ppsSize);
    jniEnv->ReleaseByteArrayElements(sps, spsData, 0);
    jniEnv->ReleaseByteArrayElements(pps, ppsData, 0);
}


static JNINativeMethod mNativeMethod[] = {
        {"NativeRtmpConnect", "(Ljava/lang/String;)V", (void *) Android_JNI_RtmpConnect},
        {"NativeRtmpClose",   "()V",                   (void *) Android_JNI_RtmpClose},
        {"pushAudio",         "([BII)V",                (void *) Android_JNI_pushAudio},
        {"pushVideo",         "([BII)V",               (void *) Android_JNI_pushVideo},
        {"pushSpsPps",        "([BI[BI)V",             (void *) Android_JNI_pushSpsPps},

};

/**
 * native
 * @param javaVM
 * @param pVoid
 * @return
 */
 int JNI_OnLoad(JavaVM *vm, void *pVoid) {
    JNIEnv *jniEnv;
    if (vm->GetEnv(reinterpret_cast<void **>(&jniEnv), JNI_VERSION_1_6)) {
        return JNI_ERR;
    }
    javaVM = vm;
    jclass jclass1 = jniEnv->FindClass(NATIVE_PUSH);
    jniEnv->RegisterNatives(jclass1, mNativeMethod, sizeof(mNativeMethod) / sizeof(mNativeMethod[0]));
    jniEnv->DeleteLocalRef(jclass1);
    return JNI_VERSION_1_6;
}


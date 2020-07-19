//
// Created by 阳坤 on 2020-07-17.
//

#include "JavaCallback.h"

JavaCallback::JavaCallback(JavaVM *vm, JNIEnv *env, jobject obj) {
    this->javaVM = vm;
    this->jniEnv = env;
    //必须声明全局 不然会报 error JNI DETECTED ERROR IN APPLICATION: use of invalid jobject 0xff868d8c
    this->jobject1 = env->NewGlobalRef(obj);// 坑，需要是全局（jobject一旦涉及到跨函数，跨线程，必须是全局引用）
    if (!jniEnv || !jobject1)
        return;

    jclass jcls = jniEnv->GetObjectClass(this->jobject1);
    this->jmid_connecting = jniEnv->GetMethodID(jcls, "onConnecting", "()V");
    this->jmid_success = jniEnv->GetMethodID(jcls, "onConnected", "()V");
    this->jmid_close = jniEnv->GetMethodID(jcls, "onClose", "()V");
    this->jmid_fail = jniEnv->GetMethodID(jcls, "onError", "(I)V");

}

JavaCallback::~JavaCallback() {
    if (jniEnv && jobject1 && javaVM) {
        jniEnv->DeleteGlobalRef(this->jobject1);
//        javaVM = NULL;
        jniEnv = NULL;
    }
}

void JavaCallback::onConnecting(int threadType) {
    if (threadType == THREAD_CHILD) {
        JNIEnv *jniEnv = 0;
        if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobject1, jmid_connecting);
        javaVM->DetachCurrentThread();
    } else {
        jniEnv->CallVoidMethod(jobject1, jmid_connecting);
    }

}

void JavaCallback::onClose(int threadType) {
    if (threadType == THREAD_CHILD) {
        JNIEnv *jniEnv = 0;
        if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobject1, jmid_close);
        javaVM->DetachCurrentThread();
    } else {
        jniEnv->CallVoidMethod(jobject1, jmid_close);
    }


}

void JavaCallback::onConnectSuccess() {
    JNIEnv *jniEnv = 0;
    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        return;
    }
    jniEnv->CallVoidMethod(jobject1, jmid_success);
    javaVM->DetachCurrentThread();

}

void JavaCallback::onConnectFail(int errorCode) {
    JNIEnv *jniEnv = 0;
    if (javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK) {
        return;
    }
    jniEnv->CallVoidMethod(jobject1, jmid_fail, errorCode);
    javaVM->DetachCurrentThread();
}

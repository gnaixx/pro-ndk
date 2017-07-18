//
// Created by 薛祥清 on 2017/7/17.
//
#include <android/log.h>
#include <jni.h>
#include <stdio.h>
//errno
#include <errno.h>
#include <string.h>

//日志
#define TAG "GNAIXX-NDK"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#define IOEXCEPTION "java/io/IOException"


#define MAX_LOG_MSG_LENGTH 256
#define MAX_BUFFER_SIZE 80

static void logMessage(JNIEnv *env, jobject obj, const char *format, ...) {
    static jmethodID methodID = NULL;
    if (NULL == methodID) {
        jclass clazz = env->GetObjectClass(obj);
        methodID = env->GetMethodID(clazz, "logMessage", "(Ljava/lang/String;)V");
        env->DeleteLocalRef(clazz);
    }
    if(methodID != NULL) {
        char buffer[MAX_LOG_MSG_LENGTH];
        va_list ap;
        va_start(ap, format); //指向 format 后可变参数的地址
        vsnprintf(buffer, MAX_LOG_MSG_LENGTH, format, ap);
        va_end(ap);

        jstring message = env->NewStringUTF(buffer);
        if (message != NULL) {
            env->CallVoidMethod(obj, methodID, message);
            env->DeleteLocalRef(message);
        }
    }
}

static void throwException(JNIEnv *env, const char *className, const char *message) {
    jclass clazz = env->FindClass(className);
    if (clazz != NULL) {
        env->ThrowNew(clazz, message);
        env->DeleteLocalRef(clazz);
    }
}

static void throwErrnoException(JNIEnv *env, const char *className, int errnum) {
    char buffer[MAX_LOG_MSG_LENGTH];
    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MSG_LENGTH)) {
        strerror_r(errno, buffer, MAX_LOG_MSG_LENGTH);
    }
    throwException(env, className, buffer);
}



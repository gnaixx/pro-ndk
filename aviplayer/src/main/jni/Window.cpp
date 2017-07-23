//
// Created by 薛祥清 on 2017/7/23.
//


extern "C" {
#include <avilib.h>
}

#include "Common.h"
#include "Window.h"

#include <android/native_window.h>
#include <android/native_window_jni.h>

void
Java_cc_gnaixx_aviplayer_WindowPlayerActivity_init(JNIEnv *env, jclass clazz, jlong avi, jobject surface){
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    if(nativeWindow == NULL){
        throwException(env, IOEXCEPTION, "Unable to get native window from surface.");
        goto exit;
    }

    if(ANativeWindow_setBuffersGeometry(nativeWindow,
                                        AVI_video_width((avi_t *) avi),
                                        AVI_video_height((avi_t *) avi),
                                        WINDOW_FORMAT_RGB_565) < 0){
        throwException(env, IOEXCEPTION, "Unable to set buffers geometry.");
    }
    ANativeWindow_release(nativeWindow);
    nativeWindow = 0;
    exit:
    return;
}

JNIEXPORT jboolean JNICALL
Java_cc_gnaixx_aviplayer_WindowPlayerActivity_render(JNIEnv *env, jclass clazz, jlong avi, jobject surface){
    jboolean isFrameRead = JNI_FALSE;
    long frameSize = 0;
    int keyFrame = 0;

    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    if(nativeWindow == NULL){
        throwException(env, IOEXCEPTION, "Unable to get native window from surface.");
        goto exit;
    }

    ANativeWindow_Buffer windowBuffer;
    if(ANativeWindow_lock(nativeWindow, &windowBuffer, 0)){
        throwException(env, IOEXCEPTION, "Unable to lock native window.");
        goto release;
    }

    frameSize = AVI_read_frame((avi_t *) avi, (char *)windowBuffer.bits, &keyFrame);
    if(frameSize > 0){
        isFrameRead = JNI_TRUE;
    }

    if(ANativeWindow_unlockAndPost(nativeWindow)){
        throwException(env, IOEXCEPTION, "Unable to unlock and post to native window.");
        goto release;
    }

    release:
    ANativeWindow_release(nativeWindow);
    nativeWindow = 0;
    exit:
    return isFrameRead;
}
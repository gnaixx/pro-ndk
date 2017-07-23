//
// Created by 薛祥清 on 2017/7/19.

extern "C" {
#include <avilib.h>
#ifdef MY_ANDROID_NDK_PROFILER_ENABLED
#include <wavlib.h>
#endif
}
#include "AviPlayer.h"

#include "Common.h"



jlong
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_open(JNIEnv *env, jclass clazz, jstring jFileName) {
    avi_t *avi = NULL;
    const char *cFileName = env->GetStringUTFChars(jFileName, NULL);
    if (cFileName == NULL) {
        goto exit;
    }

    avi = AVI_open_input_file(cFileName, 1); //打开 avi 文件
    env->ReleaseStringUTFChars(jFileName, cFileName);
    if (avi == NULL) {
        throwException(env, IOEXCEPTION, AVI_strerror());
    }

    exit:
    return (jlong) avi;

}

jint
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_getWidth(JNIEnv *env, jclass clazz, jlong avi) {
    return AVI_video_width((avi_t *) avi);
}

jint
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_getHeight(JNIEnv *env, jclass clazz, jlong avi) {
    return AVI_video_height((avi_t *) avi);
}

jdouble
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_getFrameRate(JNIEnv *env, jclass clazz, jlong avi) {
    return AVI_frame_rate((avi_t *) avi);
}

void
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_close(JNIEnv *env, jclass clazz, jlong avi) {
    AVI_close((avi_t *) avi);
}

//渲染帧
jboolean
Java_cc_gnaixx_aviplayer_BitmapPlayerActivity_render(JNIEnv *env, jclass clazz, jlong avi, jobject jBitmap) {
    jboolean isFrameRead = JNI_FALSE;

    char* frameBuffer = 0;
    long frameSize = 0;
    int keyFrame = 0;

    //锁定 bitmap 得到 raw byte
    if(AndroidBitmap_lockPixels(env, jBitmap, (void **) &frameBuffer) < 0){
        throwException(env, IOEXCEPTION, "Unable to lock pixels");
        goto exit;
    }

    //将 AVI 帧 byte 读到 bitmap 中
    frameSize = AVI_read_frame((avi_t *) avi, frameBuffer, &keyFrame);

    if(AndroidBitmap_unlockPixels(env, jBitmap) < 0){
        throwException(env, IOEXCEPTION, "Unable to unlock pixels");
        goto exit;
    }

    if(frameSize > 0){
        isFrameRead = JNI_TRUE;
    }

    exit:
        return isFrameRead;
}




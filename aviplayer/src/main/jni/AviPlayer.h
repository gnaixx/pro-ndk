//
// Created by 薛祥清 on 2017/7/19.
//

#include <jni.h>
#ifndef PRONDK_EXPORT_H
#define PRONDK_EXPORT_H
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_open(JNIEnv *, jclass, jstring);

JNIEXPORT jint JNICALL
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_getWidth(JNIEnv *, jclass, jlong);

JNIEXPORT jint JNICALL
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_getHeight(JNIEnv *, jclass, jlong);

JNIEXPORT jdouble JNICALL
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_getFrameRate(JNIEnv *, jclass, jlong);

JNIEXPORT void JNICALL
Java_cc_gnaixx_aviplayer_AbstractPlayerActivity_close(JNIEnv *, jclass, jlong);

JNIEXPORT jboolean JNICALL
Java_cc_gnaixx_aviplayer_BitmapPlayerActivity_render(JNIEnv *, jclass, jlong, jobject);



#ifdef __cplusplus
}
#endif
#endif //PRONDK_EXPORT_H

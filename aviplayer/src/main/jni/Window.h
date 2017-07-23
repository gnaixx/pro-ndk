//
// Created by 薛祥清 on 2017/7/23.
//
#include <jni.h>
#ifndef PRONDK_WINDOW_H
#define PRONDK_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT void JNICALL
Java_cc_gnaixx_aviplayer_WindowPlayerActivity_init(JNIEnv *, jclass, jlong, jobject);

JNIEXPORT jboolean JNICALL
Java_cc_gnaixx_aviplayer_WindowPlayerActivity_render(JNIEnv *, jclass, jlong, jobject);


#ifdef __cplusplus
}
#endif


#endif //PRONDK_WINDOW_H

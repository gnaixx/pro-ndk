//
// Created by 薛祥清 on 2017/7/23.
//
#include <jni.h>
#ifndef PRONDK_OPENGL_H
#define PRONDK_OPENGL_H
#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jlong JNICALL
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_init(JNIEnv *, jclass, jint);

JNIEXPORT void JNICALL
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_initSurface(JNIEnv *, jclass, jlong, jlong);

JNIEXPORT jboolean JNICALL
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_render(JNIEnv*, jclass, jlong, jlong);

JNIEXPORT void JNICALL
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_free(JNIEnv*, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif //PRONDK_OPENGL_H

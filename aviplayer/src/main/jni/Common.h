//
// Created by 薛祥清 on 2017/7/19.
//

#ifndef PRONDK_COMMON_H
#define PRONDK_COMMON_H

#include <jni.h>
#include <android/bitmap.h>

#define IOEXCEPTION "java/io/IOException"

void throwException(JNIEnv *, const char*, const char*);


#endif //PRONDK_COMMON_H

//
// Created by 薛祥清 on 2017/7/19.
//

#include "Common.h"

void throwException(JNIEnv *env, const char* className, const char* message){
    jclass clazz = env->FindClass(className);
    if (clazz != 0) {
        env->ThrowNew(clazz, message);
        env->DeleteLocalRef(clazz);
    }
}

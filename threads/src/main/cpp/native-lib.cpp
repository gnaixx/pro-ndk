#include <jni.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>

//日志
#define TAG "GNAIXX-NDK"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)


static jmethodID gOnNativeMessage = NULL;
static JavaVM *gVm = NULL;
static jobject gObj = NULL;
static pthread_mutex_t mutex;

struct NativeWorkerArgs {
    jint id;
    jint iterations;
};

extern "C" {
JNIEXPORT void JNICALL
Java_cc_gnaixx_prondk_MainActivity_nativeInit(
        JNIEnv *env,
        jobject obj) {
    LOGI("Start nativeInit");

    if (NULL == gObj) {
        gObj = env->NewGlobalRef(obj);
        if (NULL == gObj) {
            return;
        }
    }

    if (NULL == gOnNativeMessage) {
        jclass clazz = env->GetObjectClass(obj);
        gOnNativeMessage = env->GetMethodID(clazz, "onNativeMessage", "(Ljava/lang/String;)V");
        if (NULL == gOnNativeMessage) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeExceptin");
            env->ThrowNew(exceptionClazz, "Unable to find method");
            return;
        }
    }

    if (0 != pthread_mutex_init(&mutex, NULL)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeExceptin");
        env->ThrowNew(exceptionClazz, "Unable to initialize mutex");
        return;
    }
}

JNIEXPORT void JNICALL
Java_cc_gnaixx_prondk_MainActivity_nativeFree(
        JNIEnv *env,
        jobject /* this */) {
    LOGI("Start nativeFree");

    if (NULL != gObj) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }

    if(0!= pthread_mutex_destroy(&mutex)){
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeExceptin");
        env->ThrowNew(exceptionClazz, "Unable to destroy mutex");
        return;
    }
}


JNIEXPORT void JNICALL
Java_cc_gnaixx_prondk_MainActivity_nativeWorker(
        JNIEnv *env,
        jobject obj,
        jint id,
        jint iterations) {
    LOGI("Start nativeWorker");

    if(0!= pthread_mutex_lock(&mutex)){
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeExceptin");
        env->ThrowNew(exceptionClazz, "Unable to lock mutex");
        return;
    }

    for (jint i = 0; i < iterations; i++) {
        char message[26];
        sprintf(message, "Worker %d: Iteration %d", id, i);

        jstring messageString = env->NewStringUTF(message);
        env->CallVoidMethod(obj, gOnNativeMessage, messageString);
        if (NULL != env->ExceptionOccurred()) {
            break;
        }
        sleep(1);
    }

    if(0!= pthread_mutex_unlock(&mutex)){
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeExceptin");
        env->ThrowNew(exceptionClazz, "Unable to unlock mutex");
        return;
    }
}

static void *nativeWorkThread(void *args) {
    JNIEnv *env = NULL;
    if (0 == gVm->AttachCurrentThread(&env, NULL)) {
        NativeWorkerArgs *nativeWorkerArgs = (NativeWorkerArgs *) args;
        Java_cc_gnaixx_prondk_MainActivity_nativeWorker(env, gObj, nativeWorkerArgs->id, nativeWorkerArgs->iterations);
        delete nativeWorkerArgs;
        gVm->DetachCurrentThread();
    }
    return (void *) 1;
}

JNIEXPORT void JNICALL
Java_cc_gnaixx_prondk_MainActivity_posixThreads(
        JNIEnv *env,
        jobject obj,
        jint threads,
        jint iterations) {
    LOGI("Start posixThreads");

    pthread_t *handles = new pthread_t[threads];
    for (jint i = 0; i < threads; i++) {
        NativeWorkerArgs *nativeWorkerArgs = new NativeWorkerArgs();
        nativeWorkerArgs->id = i;
        nativeWorkerArgs->iterations = iterations;

        int result = pthread_create(&handles[i], NULL, nativeWorkThread, nativeWorkerArgs);
        if (result != 0) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to create thread");
            return;
        }
    }

    for (jint i = 0; i < threads; i++) {
        void *result = NULL;
        if (0 != pthread_join(handles[i], &result)) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to join thread");
        } else {
            char message[26];
            sprintf(message, "Worker %d: returned %d", i, result);
            jstring messageString = env->NewStringUTF(message);
            env->CallVoidMethod(obj, gOnNativeMessage, messageString);
            if (NULL != env->ExceptionOccurred()) {
                break;
            }
        }
    }
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    gVm = vm;
    return JNI_VERSION_1_6;
}

}



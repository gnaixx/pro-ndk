//
// Created by 薛祥清 on 2017/7/23.
//

extern "C" {
#include <avilib.h>
}

#include "OpenGL.h"
#include "Common.h"


#include <GLES/gl.h>
#include <GLES/glext.h>
#include <malloc.h>


struct Instance {
    char *buffer;
    GLuint texture;

    Instance() :
            buffer(0),
            texture(0) {

    }
};

jlong
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_init(JNIEnv *env, jclass clazz, jint avi) {
    Instance *instance = 0;

    long frameSize = AVI_frame_size((avi_t *) avi, 0);
    if (frameSize <= 0) {
        throwException(env, IOEXCEPTION, "Unable to get the frame size.");
        goto exit;
    }

    instance = new Instance();
    if (instance == 0) {
        throwException(env, IOEXCEPTION, "Unable to allocate instance.");
        goto exit;
    }

    instance->buffer = (char *) malloc(frameSize);
    if (instance->buffer == NULL) {
        throwException(env, IOEXCEPTION, "Unable to allocate buffer.");
        delete instance;
        instance = 0;
    }

    exit:
    return (jlong) instance;
}

void
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_initSurface(JNIEnv *env, jclass clazz, jlong inst, jlong avi) {
    Instance *instance = (Instance *) inst;
    glEnable(GL_TEXTURE_2D); //启用纹理
    glGenTextures(1, &instance->texture); //生成一个纹理对象
    glBindTexture(GL_TEXTURE_2D, instance->texture); //绑定到生成的纹理上

    int frameWidth = AVI_video_width((avi_t *) avi);
    int frameHeght = AVI_video_height((avi_t *) avi);

    GLint rect[] = {0, frameHeght, frameWidth, -frameHeght};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rect);

    glColor4f(1.0, 1.0, 1.0, 1.0); //填充颜色

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameWidth, frameHeght, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, 0);
}

jboolean
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_render(JNIEnv *env, jclass clazz, jlong inst, jlong avi) {
    Instance *instance = (Instance *) inst;
    jboolean isFrameRead = JNI_FALSE;
    int keyFrame = 0;

    long frameSize = AVI_read_frame((avi_t *) avi, instance->buffer, &keyFrame); //将 AVI 帧渲染到 bitmap
    if (frameSize <= 0) {
        goto exit;
    }
    isFrameRead = JNI_TRUE;

    glTexSubImage2D( //更新帧
            GL_TEXTURE_2D,
            0,
            0,
            0,
            AVI_video_width((avi_t *) avi),
            AVI_video_height((avi_t *) avi),
            GL_RGB,
            GL_UNSIGNED_SHORT_5_6_5,
            instance->buffer);

    glDrawTexiOES(0, 0, 0, AVI_video_width((avi_t *) avi), AVI_video_height((avi_t *) avi));

    exit:
    return isFrameRead;
}


void
Java_cc_gnaixx_aviplayer_OpenGLPlayerActivity_free(JNIEnv* env, jclass clazz, jlong inst){
    Instance* instance = (Instance*) inst;
    if(instance != 0){
        free(instance->buffer);
        delete(instance);
    }
}





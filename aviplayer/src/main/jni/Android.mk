LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

#so名
LOCAL_MODULE    := AVIPlayer

#编译文件
LOCAL_SRC_FILES := \
    AviPlayer.cpp \
    OpenGL.cpp \
    Window.cpp \
    Common.cpp

#添加依赖库
LOCAL_LDLIBS += -ljnigraphics
LOCAL_LDLIBS += -lGLESv1_CM
LOCAL_LDLIBS += -landroid

#启用 GL ext 原型
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES

#使用 AVILib 静态库
LOCAL_STATIC_LIBRARIES += avilib_static

#启用 NDK profiler
MY_ANDROID_NDK_PROFILER_ENABLED := true

#如果 NDK profiler 已经被启用
ifeq ($(MY_ANDROID_NDK_PROFILER_ENABLED),true)

#显示信息
$(info GUN Profiler is enabled)

#启用监控
LOCAL_CFLAGS += -DMY_ANDROID_NDK_PROFILER_ENABLED

#使用 Enbale the monitor function 静态库
LOCAL_STATIC_LIBRARIES += android-ndk-profiler
endif

#编译动态链接库
include $(BUILD_SHARED_LIBRARY)

#如果 NDK profiler 已经被启用
ifeq ($(MY_ANDROID_NDK_PROFILER_ENABLED),true)

#导入 NDK profiler 模块
$(call import-module, android-ndk-profiler/jni)
endif

#引入 AVILib 模块
$(call import-module, transcode-1.1.7/avilib)
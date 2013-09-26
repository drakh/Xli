LOCAL_PATH := $(call my-dir)
XLI_LIB_PATH := ../../../../lib/android/$(TARGET_ARCH_ABI)


# libXli.a

include $(CLEAR_VARS)
LOCAL_MODULE := Xli

LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXli.a
include $(PREBUILT_STATIC_LIBRARY)	


# libXliMain.a

include $(CLEAR_VARS)
LOCAL_MODULE := XliMain

LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXliMain.a
include $(PREBUILT_STATIC_LIBRARY)	


# libXliMedia.a

include $(CLEAR_VARS)
LOCAL_MODULE := XliMedia

LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXliMedia.a
include $(PREBUILT_STATIC_LIBRARY)	


# libGLWindow.so

include $(CLEAR_VARS)
LOCAL_MODULE    := GLWindow

LOCAL_SRC_FILES := \
	../../src/Main.cpp \

LOCAL_C_INCLUDES := ../../../include
LOCAL_CPP_FEATURES := exceptions

LOCAL_LDLIBS := -llog -landroid -lz -lm -lEGL -lGLESv2 -lOpenSLES

LOCAL_WHOLE_STATIC_LIBRARIES := XliMain
LOCAL_STATIC_LIBRARIES := Xli XliMedia

include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)
XLI_LIB_PATH := ../../../../lib/android/$(TARGET_ARCH_ABI)


# libXli.so

include $(CLEAR_VARS)
LOCAL_MODULE := Xli
LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXli.so
include $(PREBUILT_SHARED_LIBRARY)	


# libXliMain.a

include $(CLEAR_VARS)
LOCAL_MODULE := XliMain

LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXliMain.a
include $(PREBUILT_STATIC_LIBRARY)	


# libXliMedia.so

include $(CLEAR_VARS)
LOCAL_MODULE := XliMedia

LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXliMedia.so
include $(PREBUILT_SHARED_LIBRARY)	


# libXliMedia.so

include $(CLEAR_VARS)
LOCAL_MODULE := XliSoundPlayer

LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXliSoundPlayer.so
include $(PREBUILT_SHARED_LIBRARY)	


# libXliGL.so

include $(CLEAR_VARS)
LOCAL_MODULE := XliGL
LOCAL_SRC_FILES := $(XLI_LIB_PATH)/libXliGL.so
include $(PREBUILT_SHARED_LIBRARY)

# libAudioTest.so

include $(CLEAR_VARS)
LOCAL_MODULE    := AudioTest

LOCAL_SRC_FILES := \
	../../src/Main.cpp \

LOCAL_C_INCLUDES := ../../../include

LOCAL_CFLAGS   := -fexceptions -fno-rtti -fPIC -DPIC -O3 -Wno-invalid-offsetof
LOCAL_LDLIBS   := -lm -lGLESv2 -landroid -llog

LOCAL_WHOLE_STATIC_LIBRARIES := XliMain
LOCAL_SHARED_LIBRARIES := Xli XliGL XliMedia XliSoundPlayer

include $(BUILD_SHARED_LIBRARY)




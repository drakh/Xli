include $(CLEAR_VARS)
LOCAL_MODULE    := XliMain

LOCAL_SRC_FILES := \
    ../../../src/core/android/3rdparty/android_native_app_glue.c \
    ../../../src/main/XliMain.cpp
    
LOCAL_C_INCLUDES := \
    ../../include

LOCAL_CFLAGS   := -fexceptions -fno-rtti

include $(BUILD_STATIC_LIBRARY)

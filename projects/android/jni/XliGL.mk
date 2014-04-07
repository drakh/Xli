include $(CLEAR_VARS)
LOCAL_MODULE    := XliGL

LOCAL_SRC_FILES := \
    ../../../src/gl/GLContext.cpp \
    ../../../src/gl/egl/EGLContext.cpp \

LOCAL_C_INCLUDES := \
    ../../include \

LOCAL_CFLAGS   := -fexceptions -fno-rtti
LOCAL_LDLIBS   := -lEGL -lGLESv2 -landroid

LOCAL_SHARED_LIBRARIES := Xli

include $(BUILD_SHARED_LIBRARY)

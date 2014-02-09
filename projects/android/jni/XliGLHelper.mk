include $(CLEAR_VARS)
LOCAL_MODULE    := XliGLHelper

LOCAL_SRC_FILES := \
	../../../src/gl_helper/GLHelper.cpp \
	
LOCAL_C_INCLUDES := \
	../../include \

LOCAL_CFLAGS   := -fexceptions -fno-rtti -fPIC -DPIC
LOCAL_LDLIBS   := -lGLESv2

LOCAL_SHARED_LIBRARIES := Xli XliMedia

include $(BUILD_SHARED_LIBRARY)

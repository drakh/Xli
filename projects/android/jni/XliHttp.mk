include $(CLEAR_VARS)
LOCAL_MODULE    := XliHttp

LOCAL_SRC_FILES := \
	../../../src/http/android/AHttpClient.cpp \
	
LOCAL_C_INCLUDES := \
	../../include \

LOCAL_CFLAGS   := -fexceptions -fno-rtti -fPIC -DPIC -O3
LOCAL_LDLIBS   := -llog -landroid

LOCAL_SHARED_LIBRARIES := Xli

include $(BUILD_SHARED_LIBRARY)

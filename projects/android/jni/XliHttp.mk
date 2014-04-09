include $(CLEAR_VARS)
LOCAL_MODULE    := XliHttpClient

LOCAL_SRC_FILES := \
	../../../src/http_client/android/AHttpClient.cpp \

LOCAL_C_INCLUDES := \
	../../include \s

LOCAL_CFLAGS   := -fexceptions -fno-rtti -fPIC -DPIC -O3
LOCAL_LDLIBS   := -llog -landroid

LOCAL_SHARED_LIBRARIES := Xli

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := XliDummy

LOCAL_STATIC_LIBRARIES := XliMain
LOCAL_SRC_FILES := ../../../src/Dummy.cpp
LOCAL_LDLIBS   := 

include $(BUILD_SHARED_LIBRARY)

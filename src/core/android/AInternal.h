#ifndef __XLI_A_INTERNAL_H__
#define __XLI_A_INTERNAL_H__

#include <jni.h>
#include <cstdlib>
#include <android/log.h>
#include <android/native_activity.h>

extern ANativeActivity* XliActivity;

static const char* XliAppName()
{
	const char* name = getenv("XLI_APP_NAME");
	return name ? name : "XLI";
}

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, XliAppName(), __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, XliAppName(), __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, XliAppName(), __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, XliAppName(), __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, XliAppName(), __VA_ARGS__))


#endif

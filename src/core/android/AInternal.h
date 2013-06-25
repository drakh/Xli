#ifndef __XLI_A_INTERNAL_H__
#define __XLI_A_INTERNAL_H__

#include <jni.h>
#include <cstdlib>
#include <android/log.h>
#include <android/native_activity.h>
#include <Xli/Exception.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, Xli::GetAppName(), __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, Xli::GetAppName(), __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, Xli::GetAppName(), __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, Xli::GetAppName(), __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, Xli::GetAppName(), __VA_ARGS__))

#define DEBUG_JNI // Adds error checking to JNI operations

namespace Xli
{
	extern ANativeActivity* AndroidActivity;

	static const char* GetAppName()
	{
		const char* name = getenv("XLI_APP_NAME");
		return name ? name : "XLI";
	}

	class JniHelper
	{
		JNIEnv* env;

	public:
		static void Init();

		JniHelper();

		jmethodID FindMethod(const char* className, const char* methodName, const char* methodSig);

		jobject CallObjectMethod(jobject inst, const char* name, const char* sig);

		String GetString(jobject str);
		JNIEnv* GetEnv();

		JNIEnv* operator->();
	};
}


#endif

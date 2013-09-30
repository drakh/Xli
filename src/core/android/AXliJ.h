#ifndef __XLI_A_XLIJ_H__
#define __XLI_A_XLIJ_H__

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
	class XliJ
	{

	public:
        static void MakeNoise();
        static void RaiseSoftKeyboard();
        static void HideSoftKeyboard();
        static int ShowMessageBox(const String& message, const String& caption, int buttons, int hints);
	};
}

#endif

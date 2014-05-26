#ifndef __XLI_PLATFORM_SPECIFIC_ANDROID_H__
#define __XLI_PLATFORM_SPECIFIC_ANDROID_H__

#include <android/log.h>
#include <android/native_activity.h>
#include <cstdio>
#include <cstdlib>
#include <jni.h>
#include <Xli/Window.h>

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))
#define LOGF(...) ((void)__android_log_print(ANDROID_LOG_FATAL, Xli::PlatformSpecific::AGetAppName(), __VA_ARGS__))

struct android_app;

namespace Xli
{
    typedef int JObjRef;

    namespace PlatformSpecific
    {
        extern ANativeActivity* AndroidActivity;

        XLI_INLINE const char* AGetAppName()
        {
            const char* name = getenv("XLI_APP_NAME");
            return name ? name : "XliApp";
        }        
    }
}


#endif

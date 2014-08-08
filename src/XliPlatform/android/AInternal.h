/* * Copyright (C) 2010-2014 Outracks Technologies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __XLI_PLATFORM_SPECIFIC_ANDROID_INTERNAL_H__
#define __XLI_PLATFORM_SPECIFIC_ANDROID_INTERNAL_H__

#include <android/log.h>
#include <android/native_activity.h>
#include <cstdio>
#include <cstdlib>
#include <jni.h>
#include <XliPlatform/Window.h>

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

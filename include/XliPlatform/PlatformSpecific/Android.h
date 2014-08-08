#ifndef __XLI_PLATFORM_SPECIFIC_ANDROID_H__
#define __XLI_PLATFORM_SPECIFIC_ANDROID_H__

#include <jni.h>

struct android_app;

namespace Xli
{
    namespace PlatformSpecific
    {
        /**
            \ingroup XliPlatform
        */
        class Android
        {
        public:
            static void Init(struct android_app* app);

            static void SetLogName(const char* name);

            static jobject GetJniActivity();
        };
    }
}

#endif

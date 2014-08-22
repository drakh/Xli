//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <Xli/Array.h>
#include <Xli/CoreLib.h>
#include <cstdlib>

#if defined(XLI_PLATFORM_ANDROID)
# include "../../3rdparty/android_native_app_glue/android_native_app_glue.h"
# include <XliPlatform/PlatformSpecific/Android.h>
#elif defined(XLI_PLATFORM_IOS)
# include <XliPlatform/PlatformSpecific/iOS.h>
# include <SDL_main.h>
#endif

extern int Main(const Xli::Array<Xli::String>& args);

extern "C" int main(int argc, char** argv)
{
    int result = EXIT_FAILURE;

    try
    {
        Xli::CoreLib::Init();

#if defined(XLI_PLATFORM_ANDROID)
        Xli::PlatformSpecific::Android::Init((struct android_app*)*argv);
#elif defined(XLI_PLATFORM_IOS)
        Xli::PlatformSpecific::iOS::Init();
#endif

        Xli::Array<Xli::String> args(argc);
        for (int i = 0; i < argc; i++) 
            args[i] = argv[i];

        result = Main(args);
    }
    catch (const Xli::Exception& e)
    {
        Xli::CoreLib::OnUnhandledException(e, "main");
    }
    catch (...)
    {
        Xli::Exception e("An unsupported C++ exception was thrown");
        Xli::CoreLib::OnUnhandledException(e, "main");
    }

    return result;
}

#if defined(XLI_PLATFORM_WIN32)

#include <XliPlatform/PlatformSpecific/Win32.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main(__argc, __argv);
}

#elif defined(XLI_PLATFORM_ANDROID)

extern "C" void android_main(struct android_app* app)
{
    // Make sure glue isn't stripped.
    app_dummy();
    exit(main(0, (char**)&app));
}

#endif

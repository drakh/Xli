#include <Xli/Console.h>
#include <XliPlatform/Disk.h>
#include <XliPlatform/MessageBox.h>
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
#if defined(XLI_PLATFORM_ANDROID)
        Xli::PlatformSpecific::Android::Init((struct android_app*)*argv);
#elif defined(XLI_PLATFORM_IOS)
        Xli::PlatformSpecific::iOS::Init();
#endif

        Xli::Console::Init();
        Xli::NativeFileSystem::Init();

        Xli::Array<Xli::String> args(argc);
        for (int i = 0; i < argc; i++) 
            args[i] = argv[i];

        result = Main(args);

        Xli::NativeFileSystem::Done();
        Xli::Console::Done();
    }
    catch (const Xli::Exception& e)
    {
        Xli::MessageBox::HandleException(e, "XliMain");
    }
    catch (...)
    {
        Xli::Exception e("Unhandled exception (Unknown C++ exception)");
        Xli::MessageBox::HandleException(e, "XliMain");
    }

    return result;
}

#if defined(XLI_PLATFORM_WIN32)
#include <XliPlatform/PlatformSpecific/Win32Header.h>

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

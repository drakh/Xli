#include <Xli/Console.h>
#include <XliPlatform/NativeFileSystem.h>
#include <XliPlatform/MessageBox.h>

extern int Main(const Xli::Array<Xli::String>& args);

#ifdef XLI_PLATFORM_IOS
#include <Xli/DateTime.h>
#include <XliPlatform/Time.h>
namespace Xli { void Init_iOS(); }
extern "C" void SDL_Quit();
# undef main
# define main SDL_main  // Use SDL_main on iOS
    extern "C"
#endif

int main(int argc, char** argv)
{
#ifdef XLI_PLATFORM_IOS
    Xli::Init_iOS();    // Set up NSLog on iOS
#endif
    
	// Global inits goes here
	Xli::Console::Init();
	Xli::NativeFileSystem::Init();

	Xli::Array<Xli::String> args(argc);
	for (int i = 0; i < argc; i++) args[i] = argv[i];
	int result = EXIT_FAILURE;

	try
	{
		result = Main(args);
	}
	catch (const Xli::Exception& e)
	{
/*
#ifdef XLI_PLATFORM_IOS
        {
            Xli::CharString cs = Xli::DateTime(Xli::GetTimestamp()).ToString() + ": " + e.GetMessage() + " - thrown by " + e.GetFunction() + ":" + e.GetLine();
            Xli::Managed<Xli::Stream> f = Xli::Disk->OpenFile(Xli::Disk->GetLocalAppDataDirectory() + "/error.log", Xli::FileModeAppend);
            f->Write(cs.Data(), 1, cs.Length());
            f->Write("\n\n", 1, 2);
        }
#endif
*/
		Xli::MessageBox::HandleException(e, "XliMain");
	}

	// Global shutdowns goes here
	Xli::NativeFileSystem::Shutdown();
	Xli::Console::Shutdown();
    
#ifdef XLI_PLATFORM_IOS
	//exit(result);
    //SDL_Quit();
#endif
   
	return result;
}

#ifdef XLI_PLATFORM_WINDOWS
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}

#endif

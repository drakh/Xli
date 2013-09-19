#include <Xli/Console.h>
#include <Xli/NativeFileSystem.h>
#include <Xli/MessageBox.h>
#include <cstdlib>

#ifdef XLI_PLATFORM_IOS
# include <SDL.h>
    void XliInit_iOS();
#endif

extern int Main(const Xli::Array<Xli::String>& args);

extern "C" int main(int argc, char** argv)
{
	// Global inits goes here
	Xli::Console::Init();
	Xli::NativeFileSystem::Init();

	Xli::Array<Xli::String> args(argc);
	for (int i = 0; i < argc; i++) 
		args[i] = argv[i];
	
	int result = EXIT_FAILURE;

	try
	{
#ifdef XLI_PLATFORM_IOS
        XliInit_iOS();    // Set up NSLog on iOS
#endif
		
        result = Main(args);
	}
	catch (const Xli::Exception& e)
	{
		Xli::MessageBox::HandleException(e, "XliMain");
	}
	catch (...)
	{
		Xli::Exception e("Unknown error");
		Xli::MessageBox::HandleException(e, "XliMain");
	}

	// Global shutdowns goes here
	Xli::NativeFileSystem::Done();
	Xli::Console::Done();
    
	return result;
}

#ifdef XLI_PLATFORM_WIN32
#include <Xli/Win32Header.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
}

#endif

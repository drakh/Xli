#include <Xli.h>
#include <XliGL.h>
#include <XliAudio.h>

using namespace Xli;

class GLApp: public Application
{
	Managed<GLContext> gl;
    Managed<SimpleSound> mp3;
    Managed<SimpleSound> wav;

    double touchDownTime;
    double tapTime;
    int dialogType;

public:
	GLApp()
	{
		PrintLine("Starting GLApp");
	}

	virtual ~GLApp()
	{
		PrintLine("Exiting GLApp");
	}

	virtual void OnInit(Window* wnd)
	{
		Err->WriteLine("OnInit");
        
        dialogType = 0;

		// Setup OpenGL

		this->gl = GLContext::Create(wnd, 16);

		glClearColor(1,0,0,1);


		// Print platform info

		PrintLine((String)"Time: " + DateTime::Now().ToString());
		PrintLine((String)"Time (UTC): " + DateTime::NowUtc().ToString());

		PrintLine((String)"Resolution: " + wnd->GetClientSize().ToString());

		PrintLine((String)"OpenGL Vendor: " + (const char*)glGetString(GL_VENDOR));
		PrintLine((String)"OpenGL Renderer: " + (const char*)glGetString(GL_RENDERER));
		PrintLine((String)"OpenGL Version: " + (const char*)glGetString(GL_VERSION));
		PrintLine((String)"OpenGL Multisamples: " + gl->GetMultiSamples());
		PrintLine((String)"OpenGL Swap Interval: " + gl->GetSwapInterval());
    
		PrintLine((String)"FileSystem Working Dir: " + Disk->GetCurrentDirectory());
		PrintLine((String)"FileSystem Documents: " + Disk->GetSystemDirectory(SystemDirectoryDocuments));
		PrintLine((String)"FileSystem Local AppData: " + Disk->GetSystemDirectory(SystemDirectoryLocalAppData));
		PrintLine((String)"FileSystem Roaming AppData: " + Disk->GetSystemDirectory(SystemDirectoryRoamingAppData));
		PrintLine((String)"FileSystem Temp Filename: " + Disk->CreateTempFilename());
        
        mp3 = SimpleSound::Create("ChrisZabriskieCylinderOne.mp3",true);
        wav = SimpleSound::Create("WilhelmScream.wav",true);

        mp3->Play(false);
	}

	virtual void OnLoad(Window* wnd)
	{
		Err->WriteLine("OnLoad");

		// TODO: Load something
	}

	virtual void OnDraw(Window* wnd)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// TODO: Draw something
		
		gl->SwapBuffers();

		if (wnd->GetKeyState(KeySpace))
		{
			Vector2i mousePos = wnd->GetMousePosition();
			wnd->SetTitle(String::Format("%.2lf %d %d", GetSeconds(), mousePos.X, mousePos.Y));
		}
	}

	virtual bool OnTouchDown(Window* wnd, Vector2 pos, int id)
	{
		Err->WriteLine("OnTouchDown: " + pos.ToString() + ", " + id);
        touchDownTime = GetSeconds();
		return false;
	}

	virtual bool OnTouchUp(Window* wnd, Vector2 pos, int id)
	{
		Err->WriteLine("OnTouchUp: " + pos.ToString() + ", " + id);
        
		double currentTime = GetSeconds();

        if (currentTime - touchDownTime < 0.15)
        {
            if (currentTime - tapTime < 0.3)
            {
                Err->WriteLine("Bang");
                wav->Play(false);
            }

            tapTime = currentTime;
        }
        
		return false;
	}

	virtual void OnSizeChanged(Window* wnd, Vector2i clientSize)
	{
		Err->WriteLine("OnSizeChanged: " + clientSize.ToString());
		glViewport(0, 0, clientSize.X, clientSize.Y);
		Application::OnSizeChanged(wnd, clientSize);
	}

    //[TODO] cant show message box when closing as shim is gone
	virtual bool OnClosing(Window* wnd, bool& cancel)
	{
		Err->WriteLine("OnClosing");
		cancel = MessageBox::Show(wnd, "Close?", "Close?", DialogButtonsYesNo, DialogHintQuestion | DialogHintButton2Default) == DialogResultNo;
		return true;
	}

	virtual void OnClosed(Window* wnd)
	{
		Err->WriteLine("OnClosed");
	}

	virtual void OnAppLowMemory(Window* wnd)
	{
		Err->WriteLine("OnAppLowMemory");
	}

	virtual void OnAppTerminating(Window* wnd)
	{
		Err->WriteLine("OnAppTerminating");
	}

	virtual void OnAppWillEnterForeground(Window* wnd)
	{
		Err->WriteLine("OnAppWillEnterForeground");
	}

	virtual void OnAppDidEnterForeground(Window* wnd)
	{
		Err->WriteLine("OnAppDidEnterForeground");
	}

	virtual void OnAppWillEnterBackground(Window* wnd)
	{
		Err->WriteLine("OnAppWillEnterBackground");
	}

	virtual void OnAppDidEnterBackground(Window* wnd)
	{
		Err->WriteLine("OnAppDidEnterBackground");
	}
};

int Main(const Array<String>& args)
{
    GLApp app;
	Application::Run(&app);
	return 0;
}

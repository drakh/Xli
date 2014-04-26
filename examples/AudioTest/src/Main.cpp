#include <Xli.h>
#include <XliGL.h>
#include <XliSoundPlayer.h>

using namespace Xli;

// A Very leaky example of using audio

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

        GLContextAttributes glAttribs = GLContextAttributes::Default();
        //glAttribs.Buffers = 1;
        glAttribs.Samples = 16;

        gl = GLContext::Create(wnd, glAttribs);
        gl->GetAttributes(glAttribs);
        gl->SetSwapInterval(0);
        
        glClearColor(1, 0, 0, 1);

        // assets
        
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

#include <Xli.h>
#include <XliHttp.h>
#include <XliGL.h>
#include <XliAudio.h>

using namespace Xli;

class Shouty : public HttpResponseHandler
{
    virtual void OnResponse(HttpResponse* response)
    {
        int bufSize = 60;
        char d[bufSize];
        int bytesRead = 0;
        
        Err->WriteLine("-------------------------------------");
        Err->WriteLine("Im back baby!");
        if (response->Body != 0)
        {
            Err->WriteLine("got a body");
            response->Body->AtEnd();
            // while (!response->Body->AtEnd())
            // {
            //     bytesRead = response->Body->Read(&d, 1, bufSize);
            //     Err->WriteFormat("> %*.*s\n", bytesRead, bytesRead, d);
            // }
        }
        Err->WriteLine("-------------------------------------");
    }
};

class GLApp: public Application
{
	Managed<GLContext> gl;

    // Managed<SimpleSound> sound;
    Managed<HttpResponseHandler> httpCallback;

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

        // Load some sounds
        // sound = SimpleSound::Create("test2.mp3", true);

        // 
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
			wnd->SetTitle(String::Format("%.2lf %d %d", GetTime(), mousePos.X, mousePos.Y));
		}
	}

	virtual bool OnKeyDown(Window* wnd, Key key)
	{
		Err->WriteLine("OnKeyDown: " + (String)(int)key);

		if (key == KeyF11 || (key == KeyEnter && wnd->GetKeyState(KeyCtrl)))
		{
			wnd->SetFullscreen(!wnd->IsFullscreen());
			return true;
		}
		else if (key == KeyF4 || key == KeyAlt)
		{
			// Set as handled to disable default closing behaviour on Win32 (TODO: This does not work as expected)
			return true;
		}

		return false;
	}

	virtual bool OnKeyUp(Window* wnd, Key key)
	{
		Err->WriteLine("OnKeyUp: " + (String)(int)key);
		return false;
	}

	virtual bool OnTextInput(Window* wnd, const String& text)
	{
		Err->WriteLine("OnTextInput: " + text);
		return false;
	}

	virtual bool OnMouseDown(Window* wnd, Vector2i pos, MouseButton button)
	{
		Err->WriteLine("OnMouseDown: " + pos.ToString() + ", " + (String)(int)button);
		return false;
	}

	virtual bool OnMouseUp(Window* wnd, Vector2i pos, MouseButton button)
	{
		Err->WriteLine("OnMouseUp: " + pos.ToString() + ", " + (String)(int)button);
		return false;
	}

	virtual bool OnMouseMove(Window* wnd, Vector2i pos)
	{
		Err->WriteLine("OnMouseMove: " + pos.ToString());

		if (wnd->GetMouseButtonState(MouseButtonLeft))
		{
			Vector2i size = wnd->GetClientSize();

			float x = (float)pos.X / size.X;
			float y = (float)pos.Y / size.Y;

			glClearColor(x, y, 0, 1);
			return true;
		}
		else if (wnd->GetMouseButtonState(MouseButtonMiddle))
		{
			Vector2i size = wnd->GetClientSize();

			float x = (float)pos.X / size.X;
			float y = (float)pos.Y / size.Y;

			glClearColor(x, y, x, 1);
			return true;
		}
		else if (wnd->GetMouseButtonState(MouseButtonRight))
		{
			Vector2i size = wnd->GetClientSize();

			float x = (float)pos.X / size.X;
			float y = (float)pos.Y / size.Y;

			glClearColor(y, 0, x, 1);
			return true;
		}

		return false;
	}

	virtual bool OnMouseLeave(Window* wnd, Vector2i pos)
	{
		Err->WriteLine("OnMouseLeave: " + pos.ToString());
		return false;
	}

	virtual bool OnMouseWheel(Window* wnd, Vector2i delta)
	{
		Err->WriteLine("OnMouseWheel: " + delta.ToString());
		return false;
	}

	virtual bool OnTouchDown(Window* wnd, Vector2 pos, int id)
	{
		Err->WriteLine("OnTouchDown: " + pos.ToString() + ", " + id);
        touchDownTime = GetTime();
		return false;
	}

	virtual bool OnTouchMove(Window* wnd, Vector2 pos, int id)
	{
		Err->WriteLine("OnTouchMove: " + pos.ToString() + ", " + id);
        Vector2i size = wnd->GetClientSize();
        glClearColor(pos.X / size.X, pos.Y / size.Y, 0, 1);
		return false;
	}

	virtual bool OnTouchUp(Window* wnd, Vector2 pos, int id)
	{
		Err->WriteLine("OnTouchUp: " + pos.ToString() + ", " + id);
        
        if (GetTime() - touchDownTime < 0.15)
        {
            if (GetTime() - tapTime < 0.3)
            {
                Err->WriteLine("Bang");
                //wnd->BeginTextInput((Xli::TextInputHint)0);
                // sound->Play(false);
                Shouty* callback = new Shouty();
                HttpRequest* req = HttpRequest::Create("http://bbc.co.uk", HttpGetMethod, callback);
                //req->Headers.Add("test","some more");
                req->Send();
            }
            else if (wnd->IsTextInputActive())
            {
                wnd->EndTextInput();
            }
            tapTime = GetTime();
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

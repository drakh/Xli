#include <Xli.h>
#include <XliHttpClient.h>
#include <XliGL.h>

using namespace Xli;

class EHandler : public HttpEventHandler
{
    virtual void OnRequestStateChanged(HttpRequest* request) 
    {
        HttpRequestState state = request->GetState();
        if (state == HttpRequestStateHeadersReceived)
        {
            Err->WriteFormat("statusCode: %i\n", state);
        } else if (state == HttpRequestStateDone) {
            Err->WriteFormat("state changed: %i\n", state);            
            String body = String((char*)(request->GetResponseBody()->GetDataPtr()),
                          (request->GetResponseBody()->GetSizeInBytes()));
            Err->WriteLine(body);
        } else {
            Err->WriteFormat("state changed: %i\n", state);
        }
        Err->WriteLine("-------------------------------------");
    }
    virtual void OnRequestProgress(HttpRequest* request,int position, int total, bool totalKnown,
                                   HttpTransferDirection direction) 
    {
        if (totalKnown) {
            Err->WriteFormat("progress (percentage): %i\n", (int)(100*((1.0*position)/total)));
        } else {
            Err->WriteFormat("progress: %lu, %lu\n", position, total);
            Err->WriteLine("Total length not known");
        }
        Err->WriteLine("Direction is: "+String((int)direction));
        Err->WriteLine("-------------------------------------");
    }
    virtual void OnRequestAborted(HttpRequest* request) 
    {
    }
    virtual void OnRequestTimeout(HttpRequest* request) 
    {
        Err->WriteLine("timed out");
        Err->WriteLine("-------------------------------------");
    }
    virtual void OnRequestError(HttpRequest* request) 
    {
        Err->WriteLine("Had an error:");
        Err->WriteLine("-------------------------------------");
    }
};

class GLApp: public Application
{
	Managed<GLContext> gl;

    Managed<EHandler> eventHandler;
    Managed<HttpClient> httpClient;

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
        glAttribs.Samples = 16;
		this->gl = GLContext::Create(wnd, glAttribs);
        gl->GetAttributes(glAttribs);

		glClearColor(1,0,0,1);

		// Print platform info

		PrintLine((String)"Time: " + DateTime::Now().ToString());
		PrintLine((String)"Time (UTC): " + DateTime::NowUtc().ToString());

		PrintLine((String)"FileSystem Working Dir: " + Disk->GetCurrentDirectory());
		PrintLine((String)"FileSystem Documents: " + Disk->GetSystemDirectory(SystemDirectoryDocuments));
		PrintLine((String)"FileSystem Local AppData: " + Disk->GetSystemDirectory(SystemDirectoryLocalAppData));
		PrintLine((String)"FileSystem Roaming AppData: " + Disk->GetSystemDirectory(SystemDirectoryRoamingAppData));
		PrintLine((String)"FileSystem Temp Filename: " + Disk->CreateTempFilename());

        httpClient = HttpClient::Create();
        eventHandler = new EHandler();
        httpClient->SetEventHandler(eventHandler.Get());
        
        Err->WriteLine("-------------------------------------");
	}

	virtual void OnLoad(Window* wnd)
	{
		Err->WriteLine("OnLoad");
	}

    
    
	virtual void OnDraw(Window* wnd)
	{
        //
        httpClient->Update();
        
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		gl->SwapBuffers();

		if (wnd->GetKeyState(KeySpace))
		{
			Vector2i mousePos = wnd->GetMousePosition();
			wnd->SetTitle(String::Format("%.2lf %d %d", GetSeconds(), mousePos.X, mousePos.Y));
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
        
        
        Err->WriteLine("Bang");
        HttpRequest* req = httpClient->CreateRequest("GET","http://httpbin.org/get");
        req->SetHeader("Accept", "*/*");
        req->SetHeader("ohhai","canhazdata");
        req->SendAsync();
        
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
        touchDownTime = GetSeconds();
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

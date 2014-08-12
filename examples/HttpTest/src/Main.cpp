#include <Xli.h>
#include <XliHttpClient.h>
#include <XliGL.h>
#include <XliPlatform.h>

using namespace Xli;

class EHandler : public HttpEventHandler
{
    virtual void OnRequestStateChanged(HttpRequest* request) 
    {
        HttpRequestState state = request->GetState();
        if (state == HttpRequestStateHeadersReceived)
        {
            Error->WriteFormat("statusCode: %i\n", state);
        } else if (state == HttpRequestStateDone) {
            Error->WriteFormat("state changed: %i\n", state);            
            String body = String((char*)(request->GetResponseBody()->GetPtr()),
                          (request->GetResponseBody()->GetSizeInBytes()));
            Error->WriteLine(body);
        } else {
            Error->WriteFormat("state changed: %i\n", state);
        }
        Error->WriteLine("-------------------------------------");
    }
    virtual void OnRequestProgress(HttpRequest* request,int position, int total, bool totalKnown) 
    {
        if (totalKnown) {
            Error->WriteFormat("progress (percentage): %i\n", (int)(100*((1.0*position)/total)));
        } else {
            Error->WriteFormat("progress: %lu, %lu\n", position, total);
            Error->WriteLine("Total length not known");
        }
        Error->WriteLine("-------------------------------------");
    }
    virtual void OnRequestAborted(HttpRequest* request) 
    {
    }
    virtual void OnRequestTimeout(HttpRequest* request) 
    {
        Error->WriteLine("timed out");
        Error->WriteLine("-------------------------------------");
    }
    virtual void OnRequestError(HttpRequest* request) 
    {
        Error->WriteLine("error callback called...ugh:");
        Error->WriteLine("-------------------------------------");
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
		Error->WriteLine("OnInit");
        
        dialogType = 0;

		// Setup OpenGL

        GLContextAttributes glAttribs = GLContextAttributes::Default();
        glAttribs.Samples = 16;
		this->gl = GLContext::Create(wnd, glAttribs);
        gl->GetAttributes(glAttribs);

		glClearColor(1,0,0,1);

        httpClient = HttpClient::Create();
        eventHandler = new EHandler();
        httpClient->SetEventHandler(eventHandler.Get());
        
        Error->WriteLine("-------------------------------------");
	}

	virtual void OnLoad(Window* wnd)
	{
		Error->WriteLine("OnLoad");
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
		Error->WriteLine("OnKeyDown: " + (String)(int)key);

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
		Error->WriteLine("OnKeyUp: " + (String)(int)key);
		return false;
	}

	virtual bool OnTextInput(Window* wnd, const String& text)
	{
		Error->WriteLine("OnTextInput: " + text);
		return false;
	}

	virtual bool OnMouseDown(Window* wnd, Vector2i pos, MouseButton button)
	{
		Error->WriteLine("OnMouseDown: " + pos.ToString() + ", " + (String)(int)button);
        
        
        Error->WriteLine("Bang");
        HttpRequest* req = httpClient->CreateRequest("PUT","http://httpbin.org/put");
        req->SetHeader("Accept", "*/*");
        req->SetHeader("ohhai","canhazdata");
        req->SendAsync("jam jam jam");
        
		return false;
	}

	virtual bool OnMouseUp(Window* wnd, Vector2i pos, MouseButton button)
	{
		Error->WriteLine("OnMouseUp: " + pos.ToString() + ", " + (String)(int)button);
		return false;
	}

	virtual bool OnMouseMove(Window* wnd, Vector2i pos)
	{
		Error->WriteLine("OnMouseMove: " + pos.ToString());

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
		Error->WriteLine("OnMouseLeave: " + pos.ToString());
		return false;
	}

	virtual bool OnMouseWheel(Window* wnd, Vector2i delta)
	{
		Error->WriteLine("OnMouseWheel: " + delta.ToString());
		return false;
	}

	virtual bool OnTouchDown(Window* wnd, Vector2 pos, int id)
	{
		Error->WriteLine("OnTouchDown: " + pos.ToString() + ", " + id);
        touchDownTime = GetSeconds();
		return false;
	}

	virtual bool OnTouchMove(Window* wnd, Vector2 pos, int id)
	{
		Error->WriteLine("OnTouchMove: " + pos.ToString() + ", " + id);
        Vector2i size = wnd->GetClientSize();
        glClearColor(pos.X / size.X, pos.Y / size.Y, 0, 1);
		return false;
	}

	virtual bool OnTouchUp(Window* wnd, Vector2 pos, int id)
	{
        double currentTime = GetSeconds();

        if (currentTime - touchDownTime < 0.15)
        {
            if (currentTime - tapTime < 0.3)
            {
                Error->WriteLine("Bang");
                Error->WriteLine("Bang");
                HttpRequest* req = httpClient->CreateRequest("PUT","http://httpbin.org/put");
                req->SetHeader("Accept", "*/*");
                req->SetHeader("ohhai","canhazdata");
                req->SendAsync("jam jam jam");
            }

            tapTime = currentTime;
        }
        
        return false;
	}

	virtual void OnSizeChanged(Window* wnd)
	{
		Vector2i clientSize = wnd->GetClientSize();
		Error->WriteLine("OnSizeChanged: " + clientSize.ToString());
		glViewport(0, 0, clientSize.X, clientSize.Y);
		Application::OnSizeChanged(wnd);
	}
};

int Main(const Array<String>& args)
{
    GLApp app;
	Application::Run(&app);
	return 0;
}

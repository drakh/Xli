#include <Xli/SDL2Window.h>
#include <Xli/GLHeaders.h>
#include <Xli/Unicode.h>

#include <SDL.h>
#include <SDL_syswm.h>

namespace Xli
{
	static SDL2Window* GlobalWindow = 0;

	SDL2Window::SDL2Window(int width, int height, const String& title, WindowEventHandler* eventHandler, int style)
	{
		if (GlobalWindow == 0) GlobalWindow = this;

		closed = false;
		fullscreen = false;

		if ((style & WindowStyleFullscreen) == WindowStyleFullscreen)
		{
			style &= ~WindowStyleFullscreen;
			fullscreen = true;
		}

		x = SDL_WINDOWPOS_UNDEFINED;
		y = SDL_WINDOWPOS_UNDEFINED;
		w = width;
		h = height;

		Uint32 flags = SDL_WINDOW_OPENGL;
		
		if ((style & WindowStyleBorderless) == WindowStyleBorderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}
        
		if ((style & WindowStyleResizeable) == WindowStyleResizeable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}
        
		this->eventHandler = eventHandler;

#ifdef XLI_GLES2

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

#else

        flags |= SDL_WINDOW_OPENGL;

#endif
        
#ifdef XLI_PLATFORM_IOS
      
        String orientations = "";
        
        if ((style & WindowStyleOrientationLandscapeLeft) == WindowStyleOrientationLandscapeLeft)
            orientations += "LandscapeLeft ";
        
        if ((style & WindowStyleOrientationLandscapeRight) == WindowStyleOrientationLandscapeRight)
            orientations += "LandscapeRight ";
        
        if ((style & WindowStyleOrientationPortrait) == WindowStyleOrientationPortrait)
            orientations += "Portrait ";
        
        if ((style & WindowStyleOrientationPortraitUpsideDown) == WindowStyleOrientationPortraitUpsideDown)
            orientations += "PortraitUpsideDown";

        if (orientations.Length() > 0 && !SDL_SetHint("SDL_IOS_ORIENTATIONS", orientations.Data())) 
            ErrorPrintLine("SDL WARNING: Failed to set window orientations");
        
        if (!SDL_SetHint("SDL_HINT_IDLE_TIMER_DISABLED", "1")) 
            ErrorPrintLine("SDL WARNING: Failed to disable idle timer");

        if (fullscreen)
            flags |= SDL_WINDOW_BORDERLESS;
        
#endif
        
		window = SDL_CreateWindow(title.Data(), x, y, width, height, flags);

		SDL_SetWindowData(window, "SDL2Window", this);
        
#ifndef XLI_PLATFORM_IOS
        
		if (fullscreen)
		{
			fullscreen = false;
			SetFullscreen(true);
		}
        
#endif
	}

	SDL2Window::SDL2Window(const void* nativeHandle)
	{
		if (GlobalWindow == 0) GlobalWindow = this;

		window = SDL_CreateWindowFrom(nativeHandle);

		SDL_GetWindowSize(window, &w, &h);
		SDL_GetWindowPosition(window, &x, &y);

		closed = false;
		fullscreen = false;
	}

	SDL2Window::~SDL2Window()
	{
		if (GlobalWindow == this) GlobalWindow = 0;

		SDL_DestroyWindow(window);
	}

	WindowImplementation SDL2Window::GetImplementation()
	{
		return WindowImplementationSDL2;
	}

	void SDL2Window::Close()
	{
		if (eventHandler.IsSet())
		{
			if (!eventHandler->OnClose()) return;
			eventHandler->OnClosed();
		}

		SDL_HideWindow(window);
		closed = true;
	}

	bool SDL2Window::IsClosed()
	{
		return closed;
	}

	bool SDL2Window::IsVisible()
	{
		return true;
	}

    bool SDL2Window::IsFullscreen()
	{
		return fullscreen;
	}

	bool SDL2Window::IsMinimized()
	{
		return (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) == SDL_WINDOW_MINIMIZED;
	}

	bool SDL2Window::IsMaximized()
	{
		return (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) == SDL_WINDOW_MAXIMIZED;
	}

	int SDL2Window::GetDisplayIndex()
	{
		return SDL_GetWindowDisplayIndex(window);
	}

	String SDL2Window::GetTitle()
	{
		return SDL_GetWindowTitle(window);
	}

	Vector2i SDL2Window::GetPosition()
	{
		Vector2i pos;
		SDL_GetWindowPosition(window, &pos.X, &pos.Y);
		return pos;
	}

	Vector2i SDL2Window::GetClientSize()
	{
		Vector2i size;
		SDL_GetWindowSize(window, &size.X, &size.Y);
		return size;
	}

	void* SDL2Window::GetNativeHandle()
	{
		SDL_SysWMinfo info;
		if (SDL_GetWindowWMInfo(window, &info))
		{
			// TODO: This may need to be changed for X11 platforms
			return *(void**)&info.info;
		}
		return 0;
	}

	void SDL2Window::SetTitle(const String& title)
	{
		SDL_SetWindowTitle(window, title.Data());
	}

	void SDL2Window::SetMainWindow()
	{
		GlobalWindow = this;
	}

	void SDL2Window::SetFullscreen(bool fullscreen)
	{
		if (fullscreen != this->fullscreen)
		{
			this->fullscreen = fullscreen;

#ifdef XLI_PLATFORM_IOS
            
            SDL_SetWindowFullscreen(window, fullscreen ? SDL_TRUE : SDL_FALSE);
            
#else
            
			if (fullscreen)
			{
				SDL_GetWindowPosition(window, &x, &y);
				SDL_GetWindowSize(window, &w, &h);

				int i = SDL_GetWindowDisplayIndex(window);
				if (i == -1) i = 0;

				SDL_Rect rect;
				SDL_GetDisplayBounds(i, &rect);
				SDL_SetWindowPosition(window, rect.x, rect.y);
				SDL_SetWindowSize(window, rect.w, rect.h);

				SDL_SetWindowFullscreen(window, SDL_TRUE);
			}
			else
			{
				SDL_SetWindowFullscreen(window, SDL_FALSE);
				SDL_SetWindowPosition(window, x, y);
				SDL_SetWindowSize(window, w, h);
			}
            
#endif
		}
	}

	void SDL2Window::SetPosition(Vector2i pos)
	{
		SDL_SetWindowPosition(window, pos.X, pos.Y);
	}

	void SDL2Window::SetClientSize(Vector2i size)
	{
		SDL_SetWindowSize(window, size.X, size.Y);
	}

	void SDL2Window::Minimize()
	{
		SDL_MinimizeWindow(window);
	}

	void SDL2Window::Maximize()
	{
		SDL_MinimizeWindow(window);
	}

	void SDL2Window::Restore()
	{
		SDL_RestoreWindow(window);
	}

	void SDL2Window::ShowCursor(bool show)
	{
		SDL_ShowCursor(show ? 1 : 0);
	}

	static bool inited = false;

	void Window::Init()
	{
		SDL_VideoInit(0);
		inited = true;
	}

	void Window::Shutdown()
	{
		SDL_VideoQuit();
		inited = false;
	}

	static void AssertInit()
	{
		if (!inited)
		{
			Window::Init();
			atexit(Window::Shutdown);
		}
	}

	Window* Window::GetMainWindow()
	{
		return GlobalWindow;
	}

	Vector2i Window::GetScreenSize()
	{
		SDL_DisplayMode mode;
		SDL_GetDesktopDisplayMode(0, &mode);
		return Vector2i(mode.w, mode.h);
	}

	Window* Window::Create(int width, int height, const String& title, WindowEventHandler* eventHandler, int style)
	{
		AssertInit();
		return new SDL2Window(width, height, title, eventHandler, style);
	}

	Window* Window::Adopt(void* nativeWindowHandle)
	{
		AssertInit();
		return new SDL2Window(nativeWindowHandle);
	}

	static Xli::Key SDLKeyToXliKey(const SDL_Keysym& key)
	{
		switch (key.sym)
		{
		case SDLK_BACKSPACE: return KeyBackspace;
		case SDLK_TAB: return KeyTab;
		case SDLK_RETURN: case SDLK_RETURN2: return KeyEnter;
		case SDLK_LSHIFT: case SDLK_RSHIFT: return KeyShift;
		case SDLK_LCTRL: case SDLK_RCTRL: return KeyCtrl;
		case SDLK_LALT: case SDLK_RALT: return KeyAlt;
		case SDLK_PAUSE: return KeyBreak;
		case SDLK_CAPSLOCK: return KeyCapsLock;
		case SDLK_ESCAPE: return KeyEscape;
		case SDLK_SPACE: return KeySpace;
		case SDLK_PAGEUP: return KeyPageUp;
		case SDLK_PAGEDOWN: return KeyPageDown;
		case SDLK_END: return KeyEnd;
		case SDLK_HOME: return KeyHome;
		case SDLK_LEFT: return KeyLeft;
		case SDLK_UP: return KeyUp;
		case SDLK_RIGHT: return KeyRight;
		case SDLK_DOWN: return KeyDown;
		case SDLK_INSERT: return KeyInsert;
		case SDLK_DELETE: return KeyDelete;
		case SDLK_0: return Key0;
		case SDLK_1: return Key1;
		case SDLK_2: return Key2;
		case SDLK_3: return Key3;
		case SDLK_4: return Key4;
		case SDLK_5: return Key5;
		case SDLK_6: return Key6;
		case SDLK_7: return Key7;
		case SDLK_8: return Key8;
		case SDLK_9: return Key9;
		case SDLK_a: return KeyA;
		case SDLK_b: return KeyB;
		case SDLK_c: return KeyC;
		case SDLK_d: return KeyD;
		case SDLK_e: return KeyE;
		case SDLK_f: return KeyF;
		case SDLK_g: return KeyG;
		case SDLK_h: return KeyH;
		case SDLK_i: return KeyI;
		case SDLK_j: return KeyJ;
		case SDLK_k: return KeyK;
		case SDLK_l: return KeyL;
		case SDLK_m: return KeyM;
		case SDLK_n: return KeyN;
		case SDLK_o: return KeyO;
		case SDLK_p: return KeyP;
		case SDLK_q: return KeyQ;
		case SDLK_r: return KeyR;
		case SDLK_s: return KeyS;
		case SDLK_t: return KeyT;
		case SDLK_u: return KeyU;
		case SDLK_v: return KeyV;
		case SDLK_w: return KeyW;
		case SDLK_x: return KeyX;
		case SDLK_y: return KeyY;
		case SDLK_z: return KeyZ;
		case SDLK_KP_0: return KeyNumPad0;
		case SDLK_KP_1: return KeyNumPad1;
		case SDLK_KP_2: return KeyNumPad2;
		case SDLK_KP_3: return KeyNumPad3;
		case SDLK_KP_4: return KeyNumPad4;
		case SDLK_KP_5: return KeyNumPad5;
		case SDLK_KP_6: return KeyNumPad6;
		case SDLK_KP_7: return KeyNumPad7;
		case SDLK_KP_8: return KeyNumPad8;
		case SDLK_KP_9: return KeyNumPad9;
		case SDLK_F1: return KeyF1;
		case SDLK_F2: return KeyF2;
		case SDLK_F3: return KeyF3;
		case SDLK_F4: return KeyF4;
		case SDLK_F5: return KeyF5;
		case SDLK_F6: return KeyF6;
		case SDLK_F7: return KeyF7;
		case SDLK_F8: return KeyF8;
		case SDLK_F9: return KeyF9;
		case SDLK_F10: return KeyF10;
		case SDLK_F11: return KeyF11;
		case SDLK_F12: return KeyF12;

        case SDLK_LGUI:
        case SDLK_RGUI:
		case SDLK_MENU: return KeyMenu;

		default: 
            //Xli::ErrorPrintLine("SDL WARNING: Unknown key: " + CharString::HexFromInt((int)key.sym));
            return KeyUnknown;
		}
	}

	static MouseButtons SDLButtonToXliMouseButtons(Uint8 button)
	{
		switch (button)
		{
		case SDL_BUTTON_LEFT: return MouseButtonsLeft;
		case SDL_BUTTON_RIGHT: return MouseButtonsRight;
		case SDL_BUTTON_MIDDLE: return MouseButtonsMiddle;
		case SDL_BUTTON_X1: return MouseButtonsX1;
		case SDL_BUTTON_X2: return MouseButtonsX2;
		default: return MouseButtonsNone;
		}
	}
    
	void Window::ProcessMessages()
	{
		SDL_PumpEvents();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
            //ErrorPrintLine("SDL EVENT (" + String::HexFromInt((int)e.type) + ")");
            
#ifdef XLI_PLATFORM_IOS
      
            switch (e.type)
            {
                case SDL_FINGERDOWN:
                case SDL_FINGERMOTION:
                case SDL_FINGERUP:
                    if (GlobalWindow != 0 && GlobalWindow->GetEventHandler() != 0)
                    {
                        int w, h;
                        SDL_GetWindowSize(GlobalWindow->GetSDLWindow(), &w, &h);
                        
                        float x = e.tfinger.x;
                        float y = e.tfinger.y;
                        int id = (int)e.tfinger.fingerId;
                        
                        //Xli::ErrorPrintLine(String::HexFromInt((int)e.type) + " " + (String)x + " " + y + " " + (String)(int)e.tfinger.fingerId);
                        
                        switch (e.type)
                        {
                            case SDL_FINGERDOWN:
                                GlobalWindow->GetEventHandler()->OnTouchDown(x, y, id);
                                break;
                            case SDL_FINGERMOTION:
                                GlobalWindow->GetEventHandler()->OnTouchMove(x, y, id);
                                break;
                            case SDL_FINGERUP:
                                GlobalWindow->GetEventHandler()->OnTouchUp(x, y, id);
                                break;
                        }
                    }
                    
                    continue;
                    
                case SDL_MULTIGESTURE:
                    continue;
            }
#endif
            
            if (e.type == SDL_QUIT && GlobalWindow != 0)
            {
                GlobalWindow->Close();
                continue;
            }
            
            SDL2Window* wnd = 0;
            SDL_Window* sdlwnd = SDL_GetWindowFromID(e.window.windowID);
            if (sdlwnd != 0) wnd = (SDL2Window*)SDL_GetWindowData(sdlwnd, "SDL2Window");
            
			if (wnd == 0)
			{
				ErrorPrintLine("SDL WARNING: wnd pointer was NULL in Window::ProcessMessages (" + String::HexFromInt((int)e.type) + ")");
                continue;
			}
            
			switch (e.type)
			{
				case SDL_QUIT:
					wnd->Close();
					break;
                
				case SDL_WINDOWEVENT:
					switch (e.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
							wnd->Close();
							break;
                        
						case SDL_WINDOWEVENT_RESIZED:
							if (wnd->GetEventHandler() != 0)
							{
								wnd->GetEventHandler()->OnResize(e.window.data1, e.window.data2);
							}
							break;
                        
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							if (wnd->GetEventHandler() != 0)
							{
								wnd->GetEventHandler()->OnResize(e.window.data1, e.window.data2);
							}
							break;
                        
						case SDL_WINDOWEVENT_HIDDEN:
						case SDL_WINDOWEVENT_MINIMIZED:
							break;
                        
						case SDL_WINDOWEVENT_SHOWN:
						case SDL_WINDOWEVENT_MAXIMIZED:
						case SDL_WINDOWEVENT_RESTORED:
							break;
					}
					break;
                
#ifndef XLI_PLATFORM_IOS
                    
				case SDL_KEYDOWN:
					if (wnd->GetEventHandler() != 0)
					{
						wnd->GetEventHandler()->OnKeyDown(SDLKeyToXliKey(e.key.keysym));
					}
					break;
                
				case SDL_KEYUP:
					if (wnd->GetEventHandler() != 0)
					{
						wnd->GetEventHandler()->OnKeyUp(SDLKeyToXliKey(e.key.keysym));
					}
					break;
                    
				case SDL_MOUSEBUTTONDOWN:
					if (wnd->GetEventHandler() != 0)
					{
						wnd->GetEventHandler()->OnMouseDown(e.button.x, e.button.y, SDLButtonToXliMouseButtons(e.button.button));
					}
					break;
                
				case SDL_MOUSEBUTTONUP:
					if (wnd->GetEventHandler() != 0)
					{
						wnd->GetEventHandler()->OnMouseUp(e.button.x, e.button.y, SDLButtonToXliMouseButtons(e.button.button));
					}
					break;
                
				case SDL_MOUSEMOTION:
					if (wnd->GetEventHandler() != 0)
					{
						wnd->GetEventHandler()->OnMouseMove(e.button.x, e.button.y);
					}
					break;
                
				case SDL_MOUSEWHEEL:
					if (wnd->GetEventHandler() != 0)
					{
						wnd->GetEventHandler()->OnMouseWheel(e.wheel.x, e.wheel.y);
					}
					break;
#endif
			}
		}
	}
}

#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_INPUT_MODE    (1L << 2)
#define MWM_HINTS_STATUS        (1L << 3)

#define MWM_FUNC_ALL            (1L << 0)
#define MWM_FUNC_RESIZE         (1L << 1)
#define MWM_FUNC_MOVE           (1L << 2)
#define MWM_FUNC_MINIMIZE       (1L << 3)
#define MWM_FUNC_MAXIMIZE       (1L << 4)
#define MWM_FUNC_CLOSE          (1L << 5)

#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)

#define MWM_INPUT_MODELESS 0
#define MWM_INPUT_PRIMARY_APPLICATION_MODAL 1
#define MWM_INPUT_SYSTEM_MODAL 2
#define MWM_INPUT_FULL_APPLICATION_MODAL 3
#define MWM_INPUT_APPLICATION_MODAL MWM_INPUT_PRIMARY_APPLICATION_MODAL

#define MWM_TEAROFF_WINDOW      (1L<<0)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <Xli/Hash.h>

namespace Xli
{
	inline static UInt32 Hash(::Window wnd)
	{
		return Hash((void*)wnd);
	}
}

#include <Xli.h>
#include <XliPlatform/X11Window.h>

namespace Xli
{
	static int initCount = 0;

	static ::Display* x_display;

	static HashMap< ::Window, X11Window*>* windows = 0;
	static X11Window* mainWindow = 0;

	static XEvent event;

	static Pixmap blank_pixmap;
	static Cursor blank_cursor;

	static Atom wmDeleteWindow;

	void Window::Init()
	{
		if (!initCount)
		{
			x_display = XOpenDisplay(0);

			if (!x_display)
			{
				XLI_THROW("Cannot connect to X server");
			}

			static const char nothing[] = { 0,0,0,0, 0,0,0,0 };
			blank_pixmap = XCreateBitmapFromData(x_display, DefaultRootWindow(x_display), nothing, 8,8);

			XColor black, dummy;
			XAllocNamedColor(x_display, DefaultColormap(x_display, 0), "black", &black, &dummy);
			blank_cursor = XCreatePixmapCursor(x_display, blank_pixmap, blank_pixmap, &black, &black, 0, 0);

			wmDeleteWindow = XInternAtom(x_display, "WM_DELETE_WINDOW", True);

			windows = new HashMap< ::Window, X11Window*>();
		}

		initCount++;
	}

	void Window::Shutdown()
	{
		initCount--;

		if (initCount == 0)
		{
			windows->Release();
			windows = 0;

			XCloseDisplay(x_display);
			x_display = 0;
		}
		else if (initCount < 0)
		{
			XLI_THROW_BAD_DELETE;
		}
	}

	static void AssertInit()
	{
		if (!initCount)
		{
			Window::Init();
			atexit(Window::Shutdown);
		}
	}

	X11Window::X11Window(int width, int height, const String& title, WindowEventHandler* eventHandler, int style)
	{
		this->x = 0;
		this->y = 0;
		this->width = lw = width;
		this->height = lh = height;
		this->closed = false;
		this->eventHandler = eventHandler;
		this->style = style;
		this->border = 3;
		this->sizeable = true;
		this->fs = false;

		XSync(x_display, 0);

		wnd = XCreateSimpleWindow(x_display,
			DefaultRootWindow(x_display),
			x, y,
			width, height,
			0, 0, 0xFF000000);

		XSelectInput(x_display, wnd, StructureNotifyMask | ExposureMask | ButtonPressMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ButtonMotionMask | EnterWindowMask);
		XMapWindow(x_display, wnd);
		XSetWMProtocols(x_display, wnd, &wmDeleteWindow, 1);

		SetTitle(title);
		SetStyle(style);

		windows->Add(wnd, this);

		if (!mainWindow)
		{
			mainWindow = this;
		}
	}

	X11Window::~X11Window()
	{
		if (wnd != 0) Close();
	}

	void X11Window::Close()
	{
		if (this == mainWindow)
		{
			mainWindow = 0;
		}

		windows->Remove(wnd);

		XDestroyWindow(x_display, wnd);
		XFlush(x_display);

		wnd = 0;
		closed = true;
	}

	bool X11Window::IsClosed()
	{
		return closed;
	}

	bool X11Window::IsActive()
	{
		return true;
	}

	void* X11Window::GetNativeHandle()
	{
		return (void*)wnd;
	}

	Vector2i X11Window::GetPosition()
	{
		return Vector2i(x, y);
	}

	Vector2i X11Window::GetClientSize()
	{
		return Vector2i(width, height);
	}

	int X11Window::GetStyle()
	{
	    return style;
	}

	::Window X11Window::GetX11Window()
	{
	    return wnd;
	}

	::Display* X11Window::GetX11Display()
	{
		    AssertInit();
	    return x_display;
	}

	int X11Window::GetX11ScreenIndex()
	{
		return DefaultScreen(x_display);
	}

	Vector2i Window::GetScreenSize()
	{
		AssertInit();
		::Screen* screen = DefaultScreenOfDisplay(x_display);
		return Vector2i(WidthOfScreen(screen), HeightOfScreen(screen));
	}

	Window* Window::GetMainWindow()
	{
		return mainWindow;
	}

	void X11Window::SetMainWindow()
	{
		mainWindow = this;
	}

	void X11Window::SetTitle(const String& title)
	{
	    CharString ascii(title);
		XStoreName(x_display, wnd, ascii.Data());
		XFlush(x_display);
	}

	void X11Window::SetStyle(int style)
	{
		setSizeable(style == WindowStyleReszeable);
		setBorder((style == WindowStyleFixed ? 1 : 0) | (style == WindowStyleResizeable ? 2 : 0));
		setFullscreen(style == WindowStyleFullscreen);
	}

	void X11Window::SetPosition(Vector2i pos)
	{
		if (this->x == pos.X && this->y == pos.Y) return;

		this->x = pos.X;
		this->y = pos.Y;

		XMoveWindow(x_display, wnd, pos.X, pos.Y);
		XFlush(x_display);
        XSync(x_display, False);
	}

	void X11Window::SetClientSize(Vector2i size)
	{
		if (this->width == size.X && this->height == size.Y) return;

		this->width = size.X;
		this->height = size.Y;

		bool sz = sizeable;
		if (!sz) setSizeable(true);

        //Vector2i ss = GetScreenSize();
        //setFullscreen(size.X >= ss.X && size.Y >= ss.Y);

		XResizeWindow(x_display, wnd, size.X, size.Y);
		XFlush(x_display);

		if (this->width > size.X || this->height > size.Y)
		{
			XResizeWindow(x_display, wnd, size.X, size.Y);
			XFlush(x_display);
			XSync(x_display, False);
		}

		if (!sz) setSizeable(false);

		XSync(x_display, False);
	}

	void X11Window::Minimize(bool minimize)
	{
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void X11Window::Maximize(bool maximize)
	{
		XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
	}

	void X11Window::ShowCursor(bool show)
	{
		if (show)
		{
			XUndefineCursor(x_display, wnd);
		}
		else
		{
			XDefineCursor(x_display, wnd, blank_cursor);
		}

		XFlush(x_display);
	}

	void X11Window::setSizeable(bool sizeable)
	{
		if (this->sizeable == sizeable) return;
		this->sizeable = sizeable;

		XSizeHints hints;
		memset(&hints, 0, sizeof(XSizeHints));
		hints.flags = PMinSize | PMaxSize;

		if (sizeable)
		{
			hints.min_width = 1;
			hints.max_width = 0xffff;
			hints.min_height = 1;
			hints.max_height = 0xffff;
		}
		else
		{
			hints.min_width = width;
			hints.max_width = width;
			hints.min_height = height;
			hints.max_height = height;
		}

		XSetWMNormalHints(x_display, wnd, &hints);
		XFlush(x_display);
	}

	void X11Window::setFullscreen(bool fs)
	{
		if (this->fs == fs) return;
		this->fs = fs;

        XClientMessageEvent xev;
        memset(&xev, 0, sizeof(xev));

        xev.type = ClientMessage;
        xev.message_type = XInternAtom(x_display, "_NET_WM_STATE", False);
        xev.display = x_display;
        xev.window = wnd;
        xev.format = 32;
        xev.data.l[0] = fs ? 1 : 0;
        xev.data.l[1] = XInternAtom(x_display, "_NET_WM_STATE_FULLSCREEN", False);

        XSendEvent(x_display, DefaultRootWindow(x_display), False, SubstructureRedirectMask, (XEvent*)&xev);

        XFlush(x_display);
        XSync(x_display, False);
	}

	void X11Window::setBorder(int border)
	{
		if (this->border == border) return;
		this->border = border;

		/*  GET BORDER CODE
            Atom atom = XInternAtom(x_display, "_NET_FRAME_EXTENTS", True);

			// XGetWindowProperty stuff
			Atom actual_type;
			int actual_format;
			unsigned long nitems;
			unsigned long bytes_remaining;
			unsigned char *data;
			long *data_as_long;
			int status;

			// Find the _NET_FRAME_EXTENTS, also known as the window border, including
			// the titlebar and resize grippies.
			status = XGetWindowProperty(
				x_display,
				wnd,
				atom,
				0,      // long_offset
				4,      // long_length - we expect 4 32-bit values for _NET_FRAME_EXTENTS
				False,  // delete
				AnyPropertyType,
				&actual_type,
				&actual_format,
				&nitems,
				&bytes_remaining,
				&data);

            int border_left, border_right, border_top, border_bottom;
			border_left = border_right = border_top = border_bottom = 0;

			if (status == Success) {
				if ((nitems == 4) && (bytes_remaining == 0)) {
				// Hoop-jumping to avoid gcc's "dereferencing type-punned pointer" warning
				data_as_long = (long *) ((void *) data);
				border_left   = (int) *(data_as_long++);
				border_right  = (int) *(data_as_long++);
				border_top    = (int) *(data_as_long++);
				border_bottom = (int) *(data_as_long++);
				}
				XFree (data);
			}
		*/

		Atom wmHints;
		bool set = false;

		if (border == 0)
		{
			wmHints = XInternAtom(x_display, "_MOTIF_WM_HINTS", True);
			if (wmHints != 0)
			{
				struct {
					unsigned long flags;
					unsigned long functions;
					unsigned long decorations;
					long input_mode;
					unsigned long status;
				} MWMHints = { (1L << 1), 0, 0, 0, 0 };

				XChangeProperty(x_display, wnd, wmHints, wmHints, 32, PropModeReplace, (unsigned char *)&MWMHints, sizeof(MWMHints)/sizeof(long));
				set = true;
			}

			wmHints = XInternAtom(x_display, "KWM_WIN_DECORATION", True);
			if (wmHints != 0)
			{
				long KWMHints = 0;
				XChangeProperty(x_display, wnd, wmHints, wmHints, 32, PropModeReplace, (unsigned char*)&KWMHints, sizeof(KWMHints)/sizeof(long));
				set = true;
			}

			wmHints = XInternAtom(x_display, "_WIN_HINTS", True);
			if (wmHints != 0)
			{
				long GNOMEHints = 0;
				XChangeProperty(x_display, wnd, wmHints, wmHints, 32, PropModeReplace, (unsigned char *)&GNOMEHints, sizeof(GNOMEHints)/sizeof(long));
				set = true;
			}

			if (!set)
			{
				XSetTransientForHint(x_display, wnd, DefaultRootWindow(x_display));
			}
		}
		else
		{
			wmHints = XInternAtom(x_display, "_MOTIF_WM_HINTS", True);
			if (wmHints != 0)
			{
				struct {
					unsigned long flags;
					unsigned long functions;
					unsigned long decorations;
					long input_mode;
					unsigned long status;
				} MWMHints = { 0, 0, 0, 0, 0 };

                bool sz = (border & 2) != 0;

                MWMHints.flags = MWM_HINTS_DECORATIONS | MWM_HINTS_FUNCTIONS;
                MWMHints.decorations = MWM_DECOR_BORDER | MWM_DECOR_TITLE | MWM_DECOR_MINIMIZE | (sz ? MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH : 0); //MWM_DECOR_ALL;
                MWMHints.functions = MWM_FUNC_MOVE | MWM_FUNC_CLOSE | MWM_FUNC_MINIMIZE | (sz ? MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE : 0);

                XChangeProperty(x_display, wnd, wmHints, wmHints, 32, PropModeReplace, (unsigned char*)&MWMHints, sizeof(MWMHints)/sizeof(long));

				//XDeleteProperty(x_display, wnd, wmHints);
				set = true;
			}

			wmHints = XInternAtom(x_display, "KWM_WIN_DECORATION", True);
			if (wmHints != 0)
			{
				XDeleteProperty(x_display, wnd, wmHints);
				set = true;
			}

			wmHints = XInternAtom(x_display, "_WIN_HINTS", True);
			if (wmHints != 0)
			{
				XDeleteProperty(x_display, wnd, wmHints);
				set = true;
			}

			if (!set)
			{
				XSetTransientForHint(x_display, wnd, 0);
			}
		}

		XFlush(x_display);
		XSync(x_display, False);
	}

	static int toupper(unsigned int keycode)
	{
		return keycode > 96 ? keycode - 32 : keycode;
	}

	static int translateKey(unsigned int vk, bool* specialKey)
	{
		*specialKey = true;
		switch (vk)
		{
		case XK_Tab:		return KeyTab; break;
		case XK_Return:		return KeyEnter; break;
		case XK_space:		*specialKey=false; return KeySpace; break;
		case XK_Menu:		return KeyAlt; break;
		case XK_Shift_L:
		case XK_Shift_R:	return KeyShift; break;
		case XK_Control_L:
		case XK_Control_R:	return KeyCtrl; break;
		case XK_Caps_Lock:	return KeyCapsLock; break;
		case XK_Up:			return KeyUp; break;
		case XK_Down:		return KeyDown; break;
		case XK_Left:		return KeyLeft; break;
		case XK_Right:		return KeyRight; break;
		case XK_BackSpace:	return KeyBackspace; break;
		case XK_F1:			return KeyF1; break;
		case XK_F2:			return KeyF2; break;
		case XK_F3:			return KeyF3; break;
		case XK_F4:			return KeyF4; break;
		case XK_F5:			return KeyF5; break;
		case XK_F6:			return KeyF6; break;
		case XK_F7:			return KeyF7; break;
		case XK_F8:			return KeyF8; break;
		case XK_F9:			return KeyF9; break;
		case XK_F10:		return KeyF10; break;
		case XK_F11:		return KeyF11; break;
		case XK_F12:		return KeyF12; break;
		//case XK_F13:		return KeyF13; break;
		//case XK_F14:		return KeyF14; break;
		//case XK_F15:		return KeyF15; break;
		//case XK_F16:		return KeyF16; break;
		case XK_Prior:		return KeyPageUp; break;
		case XK_Next:		return KeyPageDown; break;
		case XK_Home:		return KeyHome; break;
		case XK_End:		return KeyEnd; break;
		case XK_Insert:		return KeyInsert; break;
		case XK_Delete:		return KeyDelete; break;
		case XK_Escape:		return KeyEscape; break;
		case 0xFE03:		return 0; break; // ALT GR
		default:
			*specialKey = false;
			return vk;
			break;
		}
	}

	void Window::ProcessMessages()
	{
		// storage for XLookupString-parameters
		const int MAX_MAPPED_STRING_LENGTH = 65535;
		char buffer[MAX_MAPPED_STRING_LENGTH];
		int bufsize=MAX_MAPPED_STRING_LENGTH;
		KeySym keysym = 0;
		XComposeStatus compose;

		// some variables for generating InvokeKeyboardEvent
		unsigned int key = 0;
		unsigned int count = 0;
		bool specialKey = false;

		while (XEventsQueued(x_display,QueuedAfterReading))
		{
			XNextEvent(x_display, &event);

			X11Window* window;
			if (!windows->Get(event.xany.window, window)) continue;

			WindowEventHandler* eventHandler = window->eventHandler;

            switch (event.type)
            {
            case KeyPress:
                count = XLookupString(&event.xkey, buffer, bufsize, &keysym, &compose);
                key = translateKey((unsigned int)keysym, &specialKey);
                if (!specialKey)
                {
                    if (eventHandler) eventHandler->OnKeyDown((Key)toupper(key));
                    if (eventHandler) eventHandler->OnCharTyped((Key)key);
                }
                else
                {
                    if (eventHandler) eventHandler->OnKeyDown((Key)key);
                }
                break;

			case KeyRelease:
                count = XLookupString(&event.xkey, buffer, bufsize, &keysym, &compose);
                key = translateKey((int)keysym, &specialKey);
                if (!specialKey)
                {
                    if (eventHandler) eventHandler->OnKeyUp((Key)toupper(key));
                }
                else
                {
                    if (eventHandler) eventHandler->OnKeyUp((Key)key);
                }
                break;

			case ButtonPress:
                switch (event.xbutton.button)
                {
                    case Button1:
                        if (eventHandler) eventHandler->OnMouseDown(event.xbutton.x, event.xbutton.y, MouseButtonLeft);
                        break;
                    case Button2:
                        if (eventHandler) eventHandler->OnMouseDown(event.xbutton.x, event.xbutton.y, MouseButtonCenter);
                        break;
                    case Button3:
                        if (eventHandler) eventHandler->OnMouseDown(event.xbutton.x, event.xbutton.y, MouseButtonRight);
                        break;

					// Mouse Wheel
                    case Button4:
                        if (eventHandler) eventHandler->OnMouseWheel(event.xbutton.x, event.xbutton.y, 120);
                        break;
                    case Button5:
                        if (eventHandler) eventHandler->OnMouseWheel(event.xbutton.x, event.xbutton.y, -120);
                        break;
                }
                break;

			case ButtonRelease:
                switch (event.xbutton.button)
                {
                    case Button1:
                        if (eventHandler) eventHandler->OnMouseUp(event.xbutton.x, event.xbutton.y, MouseButtonLeft);
                        break;
                    case Button2:
                        if (eventHandler) eventHandler->OnMouseUp(event.xbutton.x, event.xbutton.y, MouseButtonCenter);
                        break;
                    case Button3:
                        if (eventHandler) eventHandler->OnMouseUp(event.xbutton.x, event.xbutton.y, MouseButtonRight);
                        break;

					// Mouse Wheel
                    case Button4:
                        if (eventHandler) eventHandler->OnMouseWheel(event.xbutton.x, event.xbutton.y, 120);
                        break;
                    case Button5:
                        if (eventHandler) eventHandler->OnMouseWheel(event.xbutton.x, event.xbutton.y, -120);
                        break;
                }
                break;

			case MotionNotify:
                if (eventHandler) eventHandler->OnMouseMove(event.xmotion.x, event.xmotion.y);
                break;

			case ConfigureNotify:
				window->x = event.xconfigure.x;
				window->y = event.xconfigure.y;
				window->width = event.xconfigure.width;
				window->height = event.xconfigure.height;
				if ((window->lw != window->width) || (window->lh != window->height))
				{
					window->lw = window->width;
					window->lh = window->height;
					if (eventHandler) eventHandler->OnResize(window->width, window->height);
				}
                break;

            case ClientMessage:
                if ((unsigned int)event.xclient.data.l[0] == wmDeleteWindow)
                {
                    window->closed = true;
                    eventHandler->OnClosed();
                }
                break;

			// TODO: Handle OnClose (can return false to abort close event)

			case FocusIn:
			case FocusOut:
			case Expose:
            default:
                break;
            }
		}
	}

	Window* Window::Create(int width, int height, const String& title, WindowEventHandler* eventHandler, int style)
	{
		AssertInit();
		return new X11Window(width, height, title, eventHandler, style);
	}

    Window* Window::Adopt(void* nativeHandle)
    {
        XLI_THROW_NOT_SUPPORTED(__FUNCTION__);
    }
}

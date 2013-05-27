#include <jni.h>
#include <errno.h>
#include <unistd.h>
#include <cstdlib>

#include <EGL/egl.h>
#include "3rdparty/android_native_app_glue.h"
#include <android/native_window.h>
#include <android/native_activity.h>
#include <android/window.h>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "XLI", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "XLI", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "XLI", __VA_ARGS__))

#include <Xli/Console.h>
#include <Xli/GLContext.h>
#include <Xli/Window.h>
#include <Xli/Display.h>

struct AAssetManager* XliAAssetManager = 0;

static struct android_app* GlobalState = 0;
static Xli::WindowEventHandler* GlobalEventHandler = 0;
static Xli::Window* GlobalWindow = 0;
static int GlobalWidth = 0;
static int GlobalHeight = 0;

static volatile int inited = 0;
static volatile int visible = 1;

namespace Xli
{
	class AWindow: public Window
	{
	public:
		AWindow()
		{
			if (GlobalEventHandler != 0)
			{
				GlobalEventHandler->AddRef();
			}
		}

		virtual ~AWindow()
		{
			if (GlobalEventHandler != 0)
			{
				GlobalEventHandler->Release();
				GlobalEventHandler = 0;
			}

			GlobalWindow = 0;
		}

		virtual WindowImplementation GetImplementation()
		{
			return WindowImplementationAndroid;
		}

		virtual void Close()
		{
			if (GlobalEventHandler != 0)
			{
				if (!GlobalEventHandler->OnClose()) return;
				GlobalEventHandler->OnClosed();
			}

			GlobalState->destroyRequested = 1;
		}

		virtual bool IsClosed()
		{
			return GlobalState->destroyRequested == 1;
		}

		virtual bool IsVisible()
		{
			return visible == 1;
		}

		virtual bool IsFullscreen()
		{
			return true;
		}

		virtual bool IsMinimized()
		{
			return false;
		}

		virtual bool IsMaximized()
		{
			return true;
		}

		virtual int GetDisplayIndex()
		{
			return 0;
		}

		virtual String GetTitle()
		{
			return "";
		}

		virtual Vector2i GetPosition() 
		{
			return Vector2i(0, 0);
		}

		virtual Vector2i GetClientSize()
		{
			return Vector2i(GlobalWidth, GlobalHeight);
		}

        virtual WindowStyle GetStyle()
		{
			return WindowStyleFullscreen;
		}

		virtual void* GetNativeHandle()
		{
			return GlobalState->window;
		}

		virtual void SetTitle(const String& title)
		{
		}

		virtual void SetMainWindow()
		{
		}

		virtual void SetFullscreen(bool fullscreen)
		{
		}

		virtual void SetPosition(Vector2i pos)
		{
		}

		virtual void SetClientSize(Vector2i size)
		{
		}

		virtual void Minimize()
		{
		}

		virtual void Maximize()
		{
		}

		virtual void Restore()
		{
		}

		virtual void ShowCursor(bool show)
		{
		}
	};

	void Window::Init()
	{
	}

	void Window::Shutdown()
	{
	}

	Window* Window::GetMainWindow()
	{
		return GlobalWindow;
	}

	Vector2i Window::GetScreenSize()
	{
		int w = ANativeWindow_getWidth(GlobalState->window);
		int h = ANativeWindow_getHeight(GlobalState->window);
		return Vector2i(w, h);
	}

	Window* Window::Create(int width, int height, const String& title, WindowEventHandler* eventHandler, int style)
	{
		if (GlobalWindow != 0)
		{
			XLI_THROW("Only one window instance is allowed on the Android platform");
		}

		GlobalWidth = width;
		GlobalHeight = height;
		GlobalEventHandler = eventHandler;
		GlobalWindow = new AWindow();
	
		return GlobalWindow;
	}

	Window* Window::Adopt(void* nativeWindowHandle)
	{
		XLI_THROW_NOT_SUPPORTED(XLI_FUNC);
	}

	void Window::ProcessMessages()
	{
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) 
		{
			if (source != NULL)
			{
				source->process(GlobalState, source);
			}
		
			if (ident == LOOPER_ID_USER)
			{
				// nothing to do yet
			}
		}

		if (GlobalState->destroyRequested != 0)
		{
			LOGI("DESTROY REQUESTED");
		}

		if (inited)
		{
			// Detect window resize / screen rotation automatically here
			int w = ANativeWindow_getWidth(GlobalState->window);
			int h = ANativeWindow_getHeight(GlobalState->window);

			if (w != GlobalWidth || h != GlobalHeight)
			{
				GlobalWidth = w;
				GlobalHeight = h;

				if (GlobalEventHandler != 0)
				{
					GlobalEventHandler->OnResize(w, h);
				}
			}
		}
	}

	void Display::Init()
	{
	}

	void Display::Shutdown()
	{
	}

	int Display::GetCount()
	{
		return 1;
	}

	Recti Display::GetRect(int index)
	{
		int w = ANativeWindow_getWidth(GlobalState->window);
		int h = ANativeWindow_getHeight(GlobalState->window);
		return Recti(0, 0, w, h);
	}

	class AOutStream: public Stream
	{
		Array<char> buf;

	public:
		virtual bool CanWrite() const
		{
			return true;
		}
		
		virtual int Write(const void* src, int elmSize, int elmCount)
		{
			for (int i = 0; i < elmCount; i++)
			{
				char c = ((char*)src)[i];
				if (c == '\n')
				{
					buf.Add('\0');
					LOGI(buf.Data());
					buf.Clear();
					continue;
				}
				buf.Add(c);
			}
			return elmCount;
		}
	};

	class AErrStream: public Stream
	{
		Array<char> buf;

	public:
		virtual bool CanWrite() const
		{
			return true;
		}

		virtual int Write(const void* src, int elmSize, int elmCount)
		{
			for (int i = 0; i < elmCount; i++)
			{
				char c = ((const char*)src)[i];
				if (c == '\n')
				{
					buf.Add('\0');
					LOGW(buf.Data());
					buf.Clear();
					continue;
				}
				buf.Add(c);
			}
			return elmCount;
		}
	};
}

static int32_t handle_input(struct android_app* app, AInputEvent* event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		if (GlobalEventHandler != 0)
		{
			int ai = AMotionEvent_getAction(event);
			int a = ai & AMOTION_EVENT_ACTION_MASK;

			switch (a)
			{
			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_POINTER_UP:
				{
					int i = (ai & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
					int id = AMotionEvent_getPointerId(event, i);
					int x = AMotionEvent_getX(event, i);
					int y = AMotionEvent_getY(event, i);

					//LOGI("TOUCH EVENT: %d  %d  %d  %d  %d", a, i, id, x, y);

					switch (a)
					{
					case AMOTION_EVENT_ACTION_DOWN:
					case AMOTION_EVENT_ACTION_POINTER_DOWN:
						GlobalEventHandler->OnTouchDown(x, y, id);
						break;

					case AMOTION_EVENT_ACTION_UP:
					case AMOTION_EVENT_ACTION_POINTER_UP:
						GlobalEventHandler->OnTouchUp(x, y, id);
						break;
					}
				}
				break;

			case AMOTION_EVENT_ACTION_MOVE:
				{
					int p = AMotionEvent_getPointerCount(event);

					for (int i = 0; i < p; i++)
					{
						int id = AMotionEvent_getPointerId(event, i);
						int x = AMotionEvent_getX(event, i);
						int y = AMotionEvent_getY(event, i);
						GlobalEventHandler->OnTouchMove(x, y, id);

						//LOGI("TOUCH MOVE: %d  %d  %d  %d  %d", a, i, id, x, y);
					}
				}
				break;

			default:
				{
					int p = AMotionEvent_getPointerCount(event);

					for (int i = 0; i < p; i++)
					{
						int id = AMotionEvent_getPointerId(event, i);
						int x = AMotionEvent_getX(event, i);
						int y = AMotionEvent_getY(event, i);
						GlobalEventHandler->OnTouchUp(x, y, id);

						//LOGI("TOUCH CANCEL: %d  %d  %d  %d  %d", a, i, id, x, y);
					}
				}
				break;
			}
		}

		return 1;
	}
	else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
	{
		if (GlobalEventHandler != 0)
		{
			if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN)
			{
				switch (AKeyEvent_getKeyCode(event))
				{
				case AKEYCODE_BACK:
					if (GlobalWindow != 0) GlobalWindow->Close();
					return 1;

				case AKEYCODE_MENU:
					GlobalEventHandler->OnKeyDown(Xli::KeyMenu);
					return 1;
				}
			}
			else if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_UP)
			{
				switch (AKeyEvent_getKeyCode(event))
				{
				case AKEYCODE_BACK:
					return 1;

				case AKEYCODE_MENU:
					GlobalEventHandler->OnKeyUp(Xli::KeyMenu);
					return 1;
				}
			}
		}

		//return 1;
	}

	return 0;
}

static const char* get_cmd_string(int32_t cmd)
{
	switch (cmd)
	{
#define CASE(x) case x: return #x;
	CASE(APP_CMD_INPUT_CHANGED);
	CASE(APP_CMD_INIT_WINDOW);
	CASE(APP_CMD_TERM_WINDOW);
	CASE(APP_CMD_WINDOW_RESIZED);
	CASE(APP_CMD_WINDOW_REDRAW_NEEDED);
	CASE(APP_CMD_CONTENT_RECT_CHANGED);
	CASE(APP_CMD_GAINED_FOCUS);
	CASE(APP_CMD_LOST_FOCUS);
	CASE(APP_CMD_CONFIG_CHANGED);
	CASE(APP_CMD_LOW_MEMORY);
	CASE(APP_CMD_START);
	CASE(APP_CMD_RESUME);
	CASE(APP_CMD_SAVE_STATE);
	CASE(APP_CMD_PAUSE);
	CASE(APP_CMD_STOP);
	CASE(APP_CMD_DESTROY);
#undef CASE
	default: return "<UNKNOWN>";
	}
}

static void handle_cmd(struct android_app* app, int32_t cmd)
{
	LOGI("INCOMING CMD: %s", get_cmd_string(cmd));

	switch (cmd)
	{
		case APP_CMD_INIT_WINDOW:
			inited = 1;
			break;

		case APP_CMD_PAUSE:
			visible = 0;
			break;

		case APP_CMD_RESUME:
			visible = 1;
			break;

		case APP_CMD_LOW_MEMORY:
			if (GlobalEventHandler) GlobalEventHandler->OnLowMemory();
			break;

		case APP_CMD_STOP:
		case APP_CMD_TERM_WINDOW:
			if (GlobalEventHandler && !GlobalState->destroyRequested)
			{
				GlobalEventHandler->OnClosed();
			}

			GlobalState->destroyRequested = 1;
			break;
	}
}

extern "C" int main(int argc, char* argv[]);

extern "C" void android_main(struct android_app* state)
{
    // Make sure glue isn't stripped.
    app_dummy();

    state->userData = 0;
    state->onAppCmd = handle_cmd;
    state->onInputEvent = handle_input;
	
	GlobalState = state;

	ANativeActivity_setWindowFlags(state->activity, AWINDOW_FLAG_FULLSCREEN | AWINDOW_FLAG_KEEP_SCREEN_ON | AWINDOW_FLAG_TURN_SCREEN_ON, 0);

	try
	{
		Xli::Out->SwitchStream(Manage(new Xli::AOutStream()));
		Xli::Err->SwitchStream(Manage(new Xli::AErrStream()));

		while (!inited)
		{
			Xli::Window::ProcessMessages();
				
			if (GlobalState->destroyRequested == 1)
			{
				LOGE("Unable to initialize window");
				exit(EXIT_FAILURE);
			}

			usleep(10000);
		}

		XliAAssetManager = GlobalState->activity->assetManager;

		int exit_code = main(0, 0);

		LOGI("Exiting with code: %d", exit_code);
		exit(exit_code);
	}
	catch (const Xli::Exception& e)
	{
		LOGE("Unhandled exception: %s", e.GetMessage().Data());
		LOGE("Thrown from: %s : %d", e.GetFunction().Data(), e.GetLine());
		exit(EXIT_FAILURE);
	}
}

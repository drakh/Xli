#include "3rdparty/android_native_app_glue.h"

#include <android/window.h>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>

#include <Xli/PlatformSpecific/Android.h>
#include <Xli/Console.h>
#include <Xli/Display.h>
#include <Xli/Window.h>

Xli::WindowEventHandler* GlobalEventHandler = 0;
Xli::Window* GlobalWindow = 0;

static struct android_app* GlobalAndroidApp = 0;
static int GlobalWidth = 0;
static int GlobalHeight = 0;

static volatile int inited = 0;
static volatile int visible = 1;

namespace Xli
{
	namespace PlatformSpecific
	{
		class AWindow: public Window
		{
		public:
			AWindow()
			{
				if (GlobalEventHandler != 0)
					GlobalEventHandler->AddRef();
			}

			virtual ~AWindow()
			{
				SetEventHandler(0);
				GlobalWindow = 0;
			}

			virtual WindowImplementation GetImplementation()
			{
				return WindowImplementationAndroid;
			}

			virtual void SetEventHandler(WindowEventHandler* handler)
			{
				if (handler != 0)
					handler->AddRef();

				if (GlobalEventHandler != 0)
					GlobalEventHandler->Release();

				GlobalEventHandler = handler;
			}

			virtual WindowEventHandler* GetEventHandler()
			{
				return GlobalEventHandler;
			}

			virtual void Close()
			{
				if (GlobalEventHandler != 0)
				{
					bool cancel = false;
					
					if (GlobalEventHandler->OnClosing(this, cancel) && cancel) 
						return;
					
					GlobalEventHandler->OnClosed(this);
				}

				GlobalAndroidApp->destroyRequested = 1;
			}

			virtual bool IsClosed()
			{
				return GlobalAndroidApp->destroyRequested == 1;
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

			virtual void* GetNativeHandle()
			{
				return GlobalAndroidApp->window;
			}

			virtual void SetTitle(const String& title)
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

	        virtual void BeginTextInput(TextInputHint hint)
	        {
	            AShim::RaiseSoftKeyboard();
	        }

	        virtual void EndTextInput()
	        {
	            AShim::HideSoftKeyboard();
	        }

	        virtual bool IsTextInputActive()
	        {
	            return AShim::KeyboardVisible();
	        }

	        virtual bool HasOnscreenKeyboardSupport()
	        {
	            return true;
	        }

	        virtual bool IsOnscreenKeyboardVisible()
	        {
	            return AShim::KeyboardVisible();
	        }

			virtual bool GetKeyState(Key key)
			{
				return false;
			}

			virtual bool GetMouseButtonState(MouseButton button)
			{
				return false;
			}

			virtual Vector2i GetMousePosition()
			{
				return Vector2i(0, 0);
			}
			
			virtual void SetMousePosition(Vector2i position)
			{
			}

			virtual SystemCursor GetSystemCursor()
			{
				return SystemCursorNone;
			}
			
			virtual void SetSystemCursor(SystemCursor cursor)
			{
			}
		};

		class ALogStream: public Stream
		{
			int prio;
			Array<char> buf;

		public:
			ALogStream(int prio)
			{
				this->prio = prio;
			}

			virtual ~ALogStream()
			{
				if (buf.Length())
				{
					buf.Add(0);
					__android_log_write(prio, AGetAppName(), buf.Data());
					buf.Clear();
				}
			}

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
						buf.Add(0);
						__android_log_write(prio, AGetAppName(), buf.Data());
						buf.Clear();
						continue;
					}

					// TODO: NULL characters? Test if java-style MUTF-8 can be used

					buf.Add(c);
				}

				return elmCount;
			}
		};

		static int32_t handle_input(struct android_app* app, AInputEvent* event)
		{
			switch (AInputEvent_getType(event))
			{
			case AINPUT_EVENT_TYPE_MOTION:
				if (GlobalEventHandler)
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

							//LOGD("TOUCH EVENT: %d  %d  %d  %d  %d", a, i, id, x, y);

							switch (a)
							{
							case AMOTION_EVENT_ACTION_DOWN:
							case AMOTION_EVENT_ACTION_POINTER_DOWN:
								GlobalEventHandler->OnTouchDown(GlobalWindow, Xli::Vector2(x, y), id);
								break;

							case AMOTION_EVENT_ACTION_UP:
							case AMOTION_EVENT_ACTION_POINTER_UP:
								GlobalEventHandler->OnTouchUp(GlobalWindow, Xli::Vector2(x, y), id);
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
								GlobalEventHandler->OnTouchMove(GlobalWindow, Xli::Vector2(x, y), id);

								//LOGD("TOUCH MOVE: %d  %d  %d  %d  %d", a, i, id, x, y);
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
								GlobalEventHandler->OnTouchUp(GlobalWindow, Xli::Vector2(x, y), id);

								//LOGD("TOUCH CANCEL: %d  %d  %d  %d  %d", a, i, id, x, y);
							}
						}
						break;
					}
				}

				break;
		
			case AINPUT_EVENT_TYPE_KEY:
				if (GlobalEventHandler)
				{
					switch (AKeyEvent_getAction(event))
					{
					case AKEY_EVENT_ACTION_DOWN:
						switch (AKeyEvent_getKeyCode(event))
						{
						case AKEYCODE_BACK:
							//if (GlobalWindow != 0) GlobalWindow->Close(); // TODO
							return 1;
							
						case AKEYCODE_MENU:
							if (GlobalEventHandler->OnKeyDown(GlobalWindow, Xli::KeyMenu))
								return 1;

							break;
						}

						break;

					case AKEY_EVENT_ACTION_UP:
						switch (AKeyEvent_getKeyCode(event))
						{
						case AKEYCODE_BACK:
							return 1;

						case AKEYCODE_MENU:
							if (GlobalEventHandler->OnKeyUp(GlobalWindow, Xli::KeyMenu))
								return 1;

							break;
						}

						break;
					}
				}

				break;
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
			}

			return "<UNKNOWN>";
		}

		static void handle_cmd(struct android_app* app, int32_t cmd)
		{
			//LOGD("INCOMING CMD: %s", get_cmd_string(cmd));

			switch (cmd)
			{
				case APP_CMD_INIT_WINDOW:
					inited = 1;
					break;

				case APP_CMD_PAUSE:
					if (GlobalEventHandler)
						GlobalEventHandler->OnAppWillEnterBackground(GlobalWindow);
				
					visible = 0;

					if (GlobalEventHandler)
						GlobalEventHandler->OnAppDidEnterBackground(GlobalWindow);

					break;

				case APP_CMD_RESUME:
					if (GlobalEventHandler)
						GlobalEventHandler->OnAppWillEnterForeground(GlobalWindow);

					visible = 1;

					if (GlobalEventHandler)
						GlobalEventHandler->OnAppDidEnterForeground(GlobalWindow);

					break;

				case APP_CMD_LOW_MEMORY:
					if (GlobalEventHandler)
						GlobalEventHandler->OnAppLowMemory(GlobalWindow);
				
					break;

				case APP_CMD_STOP:
				case APP_CMD_TERM_WINDOW:
					if (!app->destroyRequested)
					{
						app->destroyRequested = 1;

						if (GlobalEventHandler)
							GlobalEventHandler->OnAppTerminating(GlobalWindow);
					}

					break;
			}
		}

		void AInit(struct android_app* app)
		{
			app->userData = 0;
			app->onAppCmd = handle_cmd;
			app->onInputEvent = handle_input;

			GlobalAndroidApp = app;
			AndroidActivity = app->activity;

			ANativeActivity_setWindowFlags(app->activity, AWINDOW_FLAG_FULLSCREEN | AWINDOW_FLAG_KEEP_SCREEN_ON | AWINDOW_FLAG_TURN_SCREEN_ON, 0);

			AJniHelper::Init();

			Out->SwitchStream(ManagePtr(new ALogStream(ANDROID_LOG_INFO)));
			Err->SwitchStream(ManagePtr(new ALogStream(ANDROID_LOG_WARN)));

			while (!inited)
			{
				Window::ProcessMessages();
				
				if (app->destroyRequested)
				{
					LOGF("Unable to initialize window");
					exit(EXIT_FAILURE);
				}

				usleep(10000);
			}
		}
	}

	void Window::Init()
	{
	}

	void Window::Done()
	{
	}

	void Window::SetMainWindow(Window* wnd)
	{
	}

	Window* Window::GetMainWindow()
	{
		return GlobalWindow;
	}

	Vector2i Window::GetScreenSize()
	{
		int w = ANativeWindow_getWidth(GlobalAndroidApp->window);
		int h = ANativeWindow_getHeight(GlobalAndroidApp->window);
		return Vector2i(w, h);
	}

	Window* Window::Create(int width, int height, const String& title, int flags)
	{
		if (GlobalWindow != 0)
			XLI_THROW("Only one window instance is allowed on the Android platform");

		GlobalWidth = width;
		GlobalHeight = height;
		GlobalWindow = new PlatformSpecific::AWindow();
	
		return GlobalWindow;
	}

	Window* Window::Adopt(void* nativeWindowHandle)
	{
		XLI_THROW_NOT_SUPPORTED(XLI_FUNCTION);
	}

	void Window::ProcessMessages()
	{
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) 
			if (source != NULL)
				source->process(GlobalAndroidApp, source);

		if (inited && !GlobalAndroidApp->destroyRequested)
		{
			// Detect window resize / screen rotation
			int w = ANativeWindow_getWidth(GlobalAndroidApp->window);
			int h = ANativeWindow_getHeight(GlobalAndroidApp->window);

			if (w != GlobalWidth || h != GlobalHeight)
			{
				GlobalWidth = w;
				GlobalHeight = h;

				if (GlobalEventHandler)
					GlobalEventHandler->OnSizeChanged(GlobalWindow, Vector2i(w, h));
			}
		}
	}

	int Display::GetCount()
	{
		return 1;
	}

	Recti Display::GetRect(int index)
	{
		int w = ANativeWindow_getWidth(GlobalAndroidApp->window);
		int h = ANativeWindow_getHeight(GlobalAndroidApp->window);
		return Recti(0, 0, w, h);
	}

	bool Display::GetCurrentSettings(int index, DisplaySettings& settings)
	{
		return false;
	}

	void Display::GetSupportedSettings(int index, Array<DisplaySettings>& settings)
	{
	}

	bool Display::ChangeSettings(int index, const DisplaySettings& settings)
	{
		return false;
	}
}

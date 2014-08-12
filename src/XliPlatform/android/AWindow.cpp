//
// Copyright (C) 2010-2014 Outracks Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute,
// sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "../../../3rdparty/android_native_app_glue/android_native_app_glue.h"

#include <android/window.h>
#include <cstdlib>
#include <errno.h>
#include <unistd.h>

#include <XliPlatform/PlatformSpecific/Android.h>
#include "AJniHelper.h"
#include "AShim.h"
#include <Xli/MutexQueue.h>
#include <Xli/Console.h>
#include <XliPlatform/Window.h>

Xli::WindowEventHandler* GlobalEventHandler = 0;
Xli::Window* GlobalWindow = 0;

static struct android_app* GlobalAndroidApp = 0;
static volatile int GlobalInit = 0;
static int GlobalWidth = 0;
static int GlobalHeight = 0;
static int GlobalFlags = 0;

namespace Xli
{
    namespace PlatformSpecific
    {
        class AWindow: public Window
        {
        public:
            MutexQueue<WindowAction*> ctActionQueue;

            AWindow()
            {
                BeginTextInput(TextInputHintDefault);
                EndTextInput();

                int nativeFlags = 0;

                if (GlobalFlags & WindowFlagsFullscreen)
                    nativeFlags |= AWINDOW_FLAG_FULLSCREEN;

                if (GlobalFlags & WindowFlagsDisablePowerSaver)
                    nativeFlags |= AWINDOW_FLAG_KEEP_SCREEN_ON | AWINDOW_FLAG_TURN_SCREEN_ON;

                //ANativeActivity_setWindowFlags(GlobalAndroidApp->activity, nativeFlags, 0);
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
                if (GlobalAndroidApp->destroyRequested == 1 ||
                    (GlobalEventHandler != 0 && 
                     GlobalEventHandler->OnClosing(this)))
                    return;

                GlobalAndroidApp->destroyRequested = 1;

                if (GlobalEventHandler != 0)
                    GlobalEventHandler->OnClosed(this);
            }

            virtual bool IsClosed()
            {
                return GlobalAndroidApp->destroyRequested == 1;
            }

            virtual bool IsVisible()
            {
                return GlobalInit != 0;
            }

            virtual bool IsFullscreen()
            {
                return true;
            }

            virtual bool IsMinimized()
            {
                return GlobalInit == 0;
            }

            virtual bool IsMaximized()
            {
                return true;
            }

            virtual bool IsStatusBarVisible()
            {
                return true;
            }

            virtual Vector2i GetStatusBarPosition()
            {
                return Vector2i(0, 0);
            }

            virtual Vector2i GetStatusBarSize()
            {
                if (!IsStatusBarVisible())
                {
                    return Vector2i(GlobalWidth, 0);
                } else {
                    return Vector2i(GlobalWidth, AShim::GetStatusBarHeight());
                }
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

            virtual Vector2i GetOnscreenKeyboardPosition()
            {
                return Vector2i(0, GlobalHeight - AShim::GetKeyboardSize());
            }

            virtual Vector2i GetOnscreenKeyboardSize()
            {
                return Vector2i(GlobalWidth, AShim::GetKeyboardSize());
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
                    __android_log_write(prio, AGetAppName(), buf.Ptr());
                    buf.Clear();
                }
            }

            virtual bool CanWrite() const
            {
                return true;
            }

            virtual void Write(const void* src, int elmSize, int elmCount)
            {
                for (int i = 0; i < elmSize * elmCount; i++)
                {
                    char c = ((const char*)src)[i];

                    switch (c)
                    {
                    case '\n':
                        buf.Add(0);
                        __android_log_write(prio, AGetAppName(), buf.Ptr());
                        buf.Clear();
                        continue;

                    case 0:
                        buf.Add((char)(unsigned char)0xC0);
                        buf.Add((char)(unsigned char)0x80);
                        continue;
                    }

                    buf.Add(c);
                }
            }
        };

        static int32_t handle_input(struct android_app* app, AInputEvent* event)
        {
            // NDK - The NDK exposes only incomplete key data so all of that has
            //       to come from the shim callbacks.
            int32_t kcode=0;
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
                                GlobalEventHandler->OnTouchDown(GlobalWindow, Vector2(x, y), id);
                                break;

                            case AMOTION_EVENT_ACTION_UP:
                            case AMOTION_EVENT_ACTION_POINTER_UP:
                                GlobalEventHandler->OnTouchUp(GlobalWindow, Vector2(x, y), id);
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
                                GlobalEventHandler->OnTouchMove(GlobalWindow, Vector2(x, y), id);

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
                                GlobalEventHandler->OnTouchUp(GlobalWindow, Vector2(x, y), id);

                                //LOGD("TOUCH CANCEL: %d  %d  %d  %d  %d", a, i, id, x, y);
                            }
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
#ifdef XLI_DEBUG
            LOGD(get_cmd_string(cmd));
#endif
            switch (cmd)
            {
            case APP_CMD_INIT_WINDOW:
                GlobalInit = 1;
                if (GlobalEventHandler)
                    GlobalEventHandler->OnNativeHandleChanged(GlobalWindow);

                break;

            case APP_CMD_TERM_WINDOW:
                GlobalInit = 0;
                break;

            case APP_CMD_START:
                if (GlobalEventHandler)
                    GlobalEventHandler->OnAppWillEnterForeground(GlobalWindow);

                break;

            case APP_CMD_RESUME:
                if (GlobalInit) // TODO: Should actually check that shim is inited, this is not robust
                    AShim::OnResume();
                
                if (GlobalEventHandler)
                    GlobalEventHandler->OnAppDidEnterForeground(GlobalWindow);

                break;

            case APP_CMD_PAUSE:
                if (GlobalInit) // TODO: Should actually check that shim is inited, this is not robust
                    AShim::OnPause();
                
                if (GlobalEventHandler)
                    GlobalEventHandler->OnAppWillEnterBackground(GlobalWindow);
                
                break;

            case APP_CMD_STOP:
                if (GlobalFlags & WindowFlagsDisableBackgroundProcess)
                    handle_cmd(app, APP_CMD_DESTROY);
                else if (GlobalEventHandler)
                    GlobalEventHandler->OnAppDidEnterBackground(GlobalWindow);
                
                break;

            case APP_CMD_DESTROY:
                if (!app->destroyRequested)
                {
                    if (GlobalEventHandler)
                        GlobalEventHandler->OnAppTerminating(GlobalWindow);

                    app->destroyRequested = 1;

                    if (GlobalEventHandler)
                        GlobalEventHandler->OnClosed(GlobalWindow);
                }
                break;

            case APP_CMD_LOW_MEMORY:
                if (GlobalEventHandler)
                    GlobalEventHandler->OnAppLowMemory(GlobalWindow);

                break;
            }
        }

        void Android::Init(struct android_app* app)
        {
            app->userData = 0;
            app->onAppCmd = handle_cmd;
            app->onInputEvent = handle_input;

            GlobalAndroidApp = app;
            AndroidActivity = app->activity;

            Out->SetStream(ManagePtr(new ALogStream(ANDROID_LOG_INFO)));
            Error->SetStream(ManagePtr(new ALogStream(ANDROID_LOG_WARN)));

            // Wait for the native window to be initialized from another thread here. Because something in the shim expects it to be.
            // TODO: Clean up the shim and remove this for better maintainability and faster start up.
            while (!GlobalInit)
            {
                Window::ProcessMessages();

                if (GlobalAndroidApp->destroyRequested)
                {
                    LOGF("Unable to initialize window");
                    exit(EXIT_FAILURE);
                }

                usleep(10000);
            }

            AJniHelper::Init();
        }

        void Android::SetLogTag(const char* tag)
        {
            setenv("XLI_APP_NAME", tag, 1);
        }

        JavaVM* Android::GetJavaVM()
        {
            return AndroidActivity->vm;
        }

        jobject Android::GetActivity()
        {
            return AndroidActivity->clazz;
        }

        void EnqueueCrossThreadEvent(Window* wnd, WindowAction* action)
        {
            ((AWindow*)wnd)->ctActionQueue.Enqueue(action);
        }

        void ProcessCrossThreadEvents(Window* wnd)
        {
            while ((((AWindow*)wnd)->ctActionQueue.Count() > 0))
            {
                WindowAction* action = ((AWindow*)wnd)->ctActionQueue.Dequeue();
                action->Execute();
                delete action;
            }
        }
    }

    void InitWindow()
    {
    }

    void TerminateWindow()
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
        GlobalFlags = flags;
        GlobalWindow = new PlatformSpecific::AWindow();

        return GlobalWindow;
    }

    Window* Window::CreateFrom(void* nativeWindowHandle)
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

        if (GlobalInit && !GlobalAndroidApp->destroyRequested)
        {
            // Detect window resize / screen rotation
            int w = ANativeWindow_getWidth(GlobalAndroidApp->window);
            int h = ANativeWindow_getHeight(GlobalAndroidApp->window);

            if (w != GlobalWidth || h != GlobalHeight)
            {
                GlobalWidth = w;
                GlobalHeight = h;

                if (GlobalEventHandler)
                    GlobalEventHandler->OnSizeChanged(GlobalWindow);
            }
        }

        if (GlobalWindow)
            PlatformSpecific::ProcessCrossThreadEvents(GlobalWindow);
    }
}

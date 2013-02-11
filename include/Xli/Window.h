#ifndef __XLI_PLATFORM_WINDOW_H__
#define __XLI_PLATFORM_WINDOW_H__

#include <Xli/Object.h>
#include <Xli/Vector2.h>
#include <Xli/HashMap.h>
#include <Xli/Queue.h>
#include <Xli/Console.h>

namespace Xli
{
	/**
		\ingroup WM
	*/
	enum Key
	{
		KeyUnknown = 0,

		KeyBackspace = 8,
		KeyTab = 9,
		KeyEnter = 13,
		KeyShift = 16,
		KeyCtrl = 17,
		KeyAlt = 18,
		KeyBreak = 19,
		KeyCapsLock = 20,
		KeyEscape = 27,
		KeySpace = 32,
		KeyPageUp = 33,
		KeyPageDown = 34,
		KeyEnd = 35,
		KeyHome = 36,
		KeyLeft = 37,
		KeyUp = 38,
		KeyRight = 39,
		KeyDown = 40,
		KeyInsert = 45,
		KeyDelete = 46,
		Key0 = 48,
		Key1 = 49,
		Key2 = 50,
		Key3 = 51,
		Key4 = 52,
		Key5 = 53,
		Key6 = 54,
		Key7 = 55,
		Key8 = 56,
		Key9 = 57,
		KeyA = 65,
		KeyB = 66,
		KeyC = 67,
		KeyD = 68,
		KeyE = 69,
		KeyF = 70,
		KeyG = 71,
		KeyH = 72,
		KeyI = 73,
		KeyJ = 74,
		KeyK = 75,
		KeyL = 76,
		KeyM = 77,
		KeyN = 78,
		KeyO = 79,
		KeyP = 80,
		KeyQ = 81,
		KeyR = 82,
		KeyS = 83,
		KeyT = 84,
		KeyU = 85,
		KeyV = 86,
		KeyW = 87,
		KeyX = 88,
		KeyY = 89,
		KeyZ = 90,
		KeyNumPad0 = 96,
		KeyNumPad1 = 97,
		KeyNumPad2 = 98,
		KeyNumPad3 = 99,
		KeyNumPad4 = 100,
		KeyNumPad5 = 101,
		KeyNumPad6 = 102,
		KeyNumPad7 = 103,
		KeyNumPad8 = 104,
		KeyNumPad9 = 105,
		KeyF1 = 112,
		KeyF2 = 113,
		KeyF3 = 114,
		KeyF4 = 115,
		KeyF5 = 116,
		KeyF6 = 117,
		KeyF7 = 118,
		KeyF8 = 119,
		KeyF9 = 120,
		KeyF10 = 121,
		KeyF11 = 122,
		KeyF12 = 123,

		KeyMenu = 200,
	};

	/**
		\ingroup WM

		Enumerates the standard mouse buttons.
	*/
	enum MouseButtons
	{
		MouseButtonsNone = 0,

		/**
			Left mouse button
		*/
		MouseButtonsLeft = 1,

		/**
			Right mouse button
		*/
		MouseButtonsRight = 2,

		/**
			Middle mouse button
		*/
		MouseButtonsMiddle = 4,

		/**
			Extra mouse button 1 (aka Mouse4)
		*/
		MouseButtonsX1 = 8,

		/**
			Extra mouse button 2 (aka Mouse5)
		*/
		MouseButtonsX2 = 16
	};

	/**
		\ingroup WM
	*/
	enum WindowEventType
	{
		WindowEventTypeKeyDown,
		WindowEventTypeKeyUp,
		WindowEventTypeCharTyped,
		WindowEventTypeMouseDown,
		WindowEventTypeMouseUp,
		WindowEventTypeMouseMove,
		WindowEventTypeMouseLeave,
		WindowEventTypeMouseWheel,
		WindowEventTypeTouchDown,
		WindowEventTypeTouchMove,
		WindowEventTypeTouchUp,
		WindowEventTypeResize,
		WindowEventTypeClosed,
	};

	/**
		\ingroup WM
	*/
	struct WindowEvent
	{
		WindowEventType Type;
		int X, Y;
		int WheelDeltaX, WheelDeltaY;
		float TouchX, TouchY;
		MouseButtons Button;
		UInt16 Char;
		Xli::Key Key;
		int TouchPointIndex;
		WindowEvent();
	};

	/**
		\ingroup WM
	*/
	class WindowEventHandler: public Xli::Object
	{
	public:
		virtual void OnKeyDown(Key key) = 0;
		virtual void OnKeyUp(Key key) = 0;
		virtual void OnCharTyped(UInt16 c) = 0;
		virtual void OnMouseDown(int x, int y, MouseButtons button) = 0;
		virtual void OnMouseUp(int x, int y, MouseButtons button) = 0;
		virtual void OnMouseMove(int x, int y) = 0;
		virtual void OnMouseLeave(int x, int y) = 0;
		virtual void OnMouseWheel(int x, int y) = 0;
		virtual void OnTouchDown(float x, float y, int id) = 0;
		virtual void OnTouchMove(float x, float y, int id) = 0;
		virtual void OnTouchUp(float x, float y, int id) = 0;
		virtual void OnResize(int w, int h) = 0;

		/**
			Must return true if window is allowed to close
		*/
		virtual bool OnClose() = 0;

		virtual void OnClosed() = 0;

		virtual void OnLowMemory() = 0;
	};

	/**
		\ingroup WM
	*/
	class WindowCloseCallback: public Object
	{
	public:
		virtual bool OnClose() = 0;
	};

	/**
		\ingroup WM
	*/
	class WindowResizeCallback: public Object
	{
	public:
		virtual void OnResize(int w, int h) = 0;
	};

	/**
		\ingroup WM
	*/
	class WindowEventQueue: public WindowEventHandler
	{
		Xli::Queue<WindowEvent> queue;

		Xli::HashMap<UInt32, bool> keyStates;
		Xli::HashMap<UInt32, bool> buttonStates;
		Vector2i mousePosition;

		Xli::HashMap<int, Xli::Vector2> touchPoints;

	public:
		Managed<WindowCloseCallback> OnCloseHandler;
		Managed<WindowResizeCallback> OnResizeHandler;

		WindowEventQueue() {}

		virtual void OnKeyDown(Key key);
		virtual void OnKeyUp(Key key);
		virtual void OnCharTyped(UInt16 c);
		virtual void OnMouseDown(int x, int y, MouseButtons button);
		virtual void OnMouseUp(int x, int y, MouseButtons button);
		virtual void OnMouseMove(int x, int y);
		virtual void OnMouseLeave(int x, int y);
		virtual void OnMouseWheel(int x, int y);
		virtual void OnTouchDown(float x, float y, int id);
		virtual void OnTouchMove(float x, float y, int id);
		virtual void OnTouchUp(float x, float y, int id);
		virtual void OnResize(int w, int h);
		virtual bool OnClose();
		virtual void OnClosed();
		virtual void OnLowMemory();

		bool PollEvent(WindowEvent& e);

		/**
			Clears the event queue
		*/
		void FlushEvents();

		/**
			Resets the key and mouse button states
		*/
		void ResetStates();

		bool GetKeyState(Key k);
		bool GetMouseButtonState(MouseButtons b);
		Vector2i GetMousePosition();
	};

	/**
		\ingroup WM
	*/
	enum WindowStyle
	{
		WindowStyleFixed = 0, ///< Fixed size window with a title bar and minimize and close buttons
		WindowStyleBorderless = 1 << 0, ///< Borderless window without titlebar and buttons
		WindowStyleResizeable = 1 << 1, ///< Resizeable window with a title bar and minimize, maximize and close buttons
		WindowStyleFullscreen = 1 << 2, ///< Fullscreen window which should always be on top
		WindowStyleFullscreenResizeable = WindowStyleResizeable | WindowStyleFullscreen, ///< Resizeable window that opens in fullscreen mode
        
        WindowStyleOrientationLandscapeLeft = 1 << 3, ///< Enable left landscape orientation (on phones and tablets)
        WindowStyleOrientationLandscapeRight = 1 << 4, ///< Enable left landscape orientation (on phones and tablets)
        WindowStyleOrientationPortrait = 1 << 5, ///< Enable portrait orientation (on phones and tablets)
        WindowStyleOrientationPortraitUpsideDown = 1 << 6, ///< Enable upside down portrait orientation (on phones and tablets)
	};
	
	/**
		\ingroup WM
	*/
	enum WindowImplementation
	{
		WindowImplementationUnknown,
		WindowImplementationWin32,
		WindowImplementationAndroid,
		WindowImplementationSDL2,
	};

	/**
		\ingroup WM

		Represents a window in the native window system.
	*/
	class Window: public Object
	{
	public:
		virtual ~Window() {}

		/**
			Returns the window implementation type
		*/
		virtual WindowImplementation GetImplementation() = 0;

		/**
			Closes the window
		*/
		virtual void Close() = 0;

		/**
			Returns wether or not the window has been closed by the user or operating system.
		*/
		virtual bool IsClosed() = 0;

		/**
			Returns wether or not the window is currently visible (i.e. not minimized or hidden).
		*/
		virtual bool IsVisible() = 0;

		/**
			Returns if this is a fullscreen window
		*/
		virtual bool IsFullscreen() = 0;

		/**
			Returns if this window is minimized
		*/
		virtual bool IsMinimized() = 0;

		/**
			Returns if this window is maximized
		*/
		virtual bool IsMaximized() = 0;

		/**
			Returns the windows title
		*/
		virtual String GetTitle() = 0;

		/**
			Returns the position of the window
		*/
		virtual Vector2i GetPosition() = 0;

		/**
			Returns the size of the client area of the window
		*/
		virtual Vector2i GetClientSize() = 0;

		/**
			Returns the index for the display containing this windows center coordinate
		*/
		virtual int GetDisplayIndex() = 0;

		/**
			Returns the native windows handle
		*/
		virtual void* GetNativeHandle() = 0;

		/**
            Sets the windows title
		*/
		virtual void SetTitle(const String& title) = 0;

		/**
			Sets the window as main window
		*/
		virtual void SetMainWindow() = 0;

		/**
			Sets the position of the window
		*/
		virtual void SetPosition(Vector2i pos) = 0;

		/**
			Sets the size of the client area of the window
		*/
		virtual void SetClientSize(Vector2i size) = 0;

		/**
			Sets the fullscreen mode of the window. 
			The window will go fullscreen on the display which contains the windows centre coordinate and stretched to that displays resolution.
		*/
		virtual void SetFullscreen(bool fullscreen) = 0;

		/**
			Minimizes the window
		*/
		virtual void Minimize() = 0;

		/**
			Maximizes the window
		*/
		virtual void Maximize() = 0;

		/**
			Restores the window to normal position
		*/
		virtual void Restore() = 0;

		/**
			Show or hide the mouse cursor
		*/
		virtual void ShowCursor(bool show) = 0;

		/**
			Initializes the Window implementation.
			Is called automatically if XliMain is used.
		*/
		static void Init();

		/**
			Shuts down the Window implementation.
			Is called automatically if XliMain is used.
		*/
		static void Shutdown();

		/**
			Returns the first window created. Can return 0.
		*/
		static Window* GetMainWindow();

		/**
			Returns the size of the primary monitor
		*/
		static Vector2i GetScreenSize();

		/**
			Creates a window
		*/
		static Window* Create(int width, int height, const Xli::String& title, WindowEventHandler* eventHandler = 0, int style = WindowStyleFixed);

		/**
			Creates a window
		*/
		static inline Window* Create(const Vector2i& size, const Xli::String& title, WindowEventHandler* eventHandler = 0, int style = WindowStyleFixed)
		{ 
			return Create(size.X, size.Y, title, eventHandler, style);
		}

		/**
			Adopts a native window
		*/
		static Window* Adopt(void* nativeWindowHandle);

		/**
			Process messages for the application to keep user interface responsive.
		*/
		static void ProcessMessages();
	};
}

#endif

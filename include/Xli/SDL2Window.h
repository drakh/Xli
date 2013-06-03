#include <Xli/Window.h>
#include <SDL.h>

namespace Xli
{
	/**
		\ingroup WM
	*/
	class SDL2Window: public Window
	{
        friend class Window;
        
		Shared<WindowEventHandler> eventHandler;
		SDL_Window* window;
		bool closed, fullscreen;
		int x, y, w, h, buttons;

	public:
		WindowEventHandler* GetEventHandler() { return eventHandler; }
		SDL_Window* GetSDLWindow() { return window; }

		SDL2Window(int width, int height, const Xli::String& title, WindowEventHandler* eventHandler, int flags);
		SDL2Window(const void* nativeHandle);
		virtual ~SDL2Window();

		virtual WindowImplementation GetImplementation();

		virtual bool IsClosed();
		virtual bool IsVisible();
		virtual bool IsFullscreen();
		virtual bool IsMinimized();
		virtual bool IsMaximized();

        virtual int GetDisplayIndex();
		virtual String GetTitle();
		virtual Vector2i GetPosition();
		virtual Vector2i GetClientSize();
		virtual void* GetNativeHandle();

		virtual void SetTitle(const String& title);
		virtual void SetMainWindow();
		virtual void SetFullscreen(bool fullscreen);
		virtual void SetPosition(Vector2i pos);
		virtual void SetClientSize(Vector2i size);

		virtual void Close();

		virtual void Minimize();
		virtual void Maximize();
		virtual void Restore();

		virtual bool GetKeyState(Key key);
		virtual bool GetMouseButtonState(MouseButton button);
		virtual Vector2i GetMousePosition();
		virtual void SetMousePosition(Vector2i position);

		virtual SystemCursor GetSystemCursor();
		virtual void SetSystemCursor(SystemCursor cursor);

		virtual void BeginTextInput();
		virtual void EndTextInput();
		virtual bool IsTextInputActive();
		virtual bool HasOnscreenKeyboardSupport();
		virtual bool IsOnscreenKeyboardVisible();
	};
}

#include <Xli/Window.h>
#include <Xli/Thread.h>
#include <Xli/StringBuilder.h>
#include <cstring>

namespace Xli
{
	WindowEvent::WindowEvent() 
	{ 
		memset(this, 0, sizeof(WindowEvent)); 
	}

	void WindowEventQueue::OnKeyDown(Key key)
	{
		keyStates[(unsigned int)key] = true;
		WindowEvent e;
		e.Type = WindowEventTypeKeyDown;
		e.Key = key;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnKeyUp(Key key)
	{
		keyStates[(unsigned int)key] = false;
		WindowEvent e;
		e.Type = WindowEventTypeKeyUp;
		e.Key = key;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnCharTyped(UInt16 c)
	{
		WindowEvent e;
		e.Type = WindowEventTypeCharTyped;
		e.Char = c;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnMouseDown(int x, int y, MouseButtons button)
	{
		buttonStates[(unsigned int)button] = true;
		mousePosition.X = x;
		mousePosition.Y = y;
		WindowEvent e;
		e.Type = WindowEventTypeMouseDown;
		e.X = x;
		e.Y = y;
		e.Button = button;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnMouseUp(int x, int y, MouseButtons button)
	{
		buttonStates[(unsigned int)button] = false;
		mousePosition.X = x;
		mousePosition.Y = y;
		WindowEvent e;
		e.Type = WindowEventTypeMouseUp;
		e.X = x;
		e.Y = y;
		e.Button = button;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnMouseMove(int x, int y)
	{
		mousePosition.X = x;
		mousePosition.Y = y;
		WindowEvent e;
		e.Type = WindowEventTypeMouseMove;
		e.X = x;
		e.Y = y;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnMouseLeave(int x, int y)
	{
		mousePosition.X = x;
		mousePosition.Y = y;
		WindowEvent e;
		e.Type = WindowEventTypeMouseLeave;
		e.X = x;
		e.Y = y;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnMouseWheel(int x, int y)
	{
		mousePosition.X = x;
		mousePosition.Y = y;
		WindowEvent e;
		e.Type = WindowEventTypeMouseWheel;
		e.WheelDeltaX = x;
		e.WheelDeltaY = x;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnTouchDown(float x, float y, int id)
	{
		if (!touchPoints.ContainsKey(id)) touchPoints.Add(id, Vector2(x, y));
		WindowEvent e;
		e.Type = WindowEventTypeTouchDown;
		e.TouchPointIndex = id;
		e.TouchX = x;
		e.TouchY = y;
		e.X = (int) x;
		e.Y = (int) y;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnTouchMove(float x, float y, int id)
	{
		touchPoints[id] = Vector2(x, y);
		WindowEvent e;
		e.Type = WindowEventTypeTouchMove;
		e.TouchPointIndex = id;
		e.TouchX = x;
		e.TouchY = y;
		e.X = (int) x;
		e.Y = (int) y;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnTouchUp(float x, float y, int id)
	{
		if (touchPoints.ContainsKey(id)) touchPoints.Remove(id);
		WindowEvent e;
		e.Type = WindowEventTypeTouchUp;
		e.TouchPointIndex = id;
		e.TouchX = x;
		e.TouchY = y;
		e.X = (int) x;
		e.Y = (int) y;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnResize(int w, int h)
	{
		if (OnResizeHandler.IsSet())
		{
			OnResizeHandler->OnResize(w, h);
			return;
		}
		WindowEvent e;
		e.X = w;
		e.Y = h;
		e.Type = WindowEventTypeResize;
		queue.Enqueue(e);
	}

	bool WindowEventQueue::OnClose()
	{
		if (OnCloseHandler.IsSet()) return OnCloseHandler->OnClose();
		return true;
	}

	void WindowEventQueue::OnClosed()
	{
		WindowEvent e;
		e.Type = WindowEventTypeClosed;
		queue.Enqueue(e);
	}

	void WindowEventQueue::OnLowMemory()
	{
	}

	bool WindowEventQueue::PollEvent(WindowEvent& e)
	{
		if (queue.IsEmpty()) return false;
		e = queue.Dequeue();
		return true;
	}

	void WindowEventQueue::FlushEvents()
	{
		queue.Clear();
	}

	void WindowEventQueue::ResetStates()
	{
		keyStates.Clear();
		buttonStates.Clear();
	}

	bool WindowEventQueue::GetKeyState(Key k)
	{
		bool b = false;
		keyStates.TryGetValue((unsigned int)k, b);
		return b;
	}

	bool WindowEventQueue::GetMouseButtonState(MouseButtons b)
	{
		bool s = false;
		buttonStates.TryGetValue((unsigned int)b, s);
		return s;
	}

	Vector2i WindowEventQueue::GetMousePosition()
	{
		return mousePosition;
	}
}

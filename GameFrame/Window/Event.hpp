#ifndef GAMEFRAME_WINDOW_EVENT_HPP
#define GAMEFRAME_WINDOW_EVENT_HPP

/*
	Events will be used during window's input handling
*/

#include "Keyboard.hpp"
#include "Mouse.hpp"

#include <variant>

namespace HJUIK
{
	namespace EventType
	{
		// Event struct lists
		// #1 Window changed size
		struct Resize
		{
			unsigned int Width;
			unsigned int Height;
		};
		// #2 Window closed
		struct Close { };
		// #3 Window gained focus
		struct GainFocus { };
		// #4 Window lost focus
		struct LoseFocus { };
		// #5 Keyboard pressed
		struct KeyPress
		{
			// Actually scancode, but we only care about it so "Code" for short
			Window::Keyboard::Key Code;
		};
		// #6 Keyboard released
		struct KeyRelease
		{
			Window::Keyboard::Key Code;
		};
		// #7 Mouse button pressed
		struct MousePress
		{
			Window::Mouse::Button Button;
		};
		// #8 Mouse button released
		struct MouseRelease
		{
			Window::Mouse::Button Button;
		};
		// #9 Mouse wheel scrolled
		struct MouseScroll
		{
			// The offset of the mouse wheel. Positive value means scrolling up.
			float Delta;
		};
		// #10 Mouse moved
		struct MouseMove
		{
			int X;
			int Y;
		};
	} // namespace EventType

	// Represent system events happening during runtime
	class Event
	{
	public:
		using Data = std::variant<
			EventType::Resize,
			EventType::Close,
			EventType::GainFocus,
			EventType::LoseFocus,
			EventType::KeyPress,
			EventType::KeyRelease,
			EventType::MousePress,
			EventType::MouseRelease,
			EventType::MouseScroll,
			EventType::MouseMove>;
		Data& getData();
	private:
		Data mData;
	};
} // namespace HJUIK

#endif
#ifndef __WINDOW_EVENT_HPP__
#define __WINDOW_EVENT_HPP__

/*
	The abstract event class. Used for window's input.
*/

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace HJUIK
{
	namespace Window
	{
		struct Event
		{
			// Size of window after resizing
			struct Size
			{
				unsigned int x;
				unsigned int y;
			} size;

			// Position of mouse after moved
			struct MousePosition
			{
				int x;
				int y;
			} mousePos;

			// Scroll delta of mouse
			float mouseScrollDelta; // Positive value means upwards movement

			// Button clicked
			Mouse::Button button;

			// Key pressed
			Keyboard::Key key;

			// Enum instead of enum class for shorter typing
			enum Type
			{
				// Window
				Resized = 0, Closed, LostFocus, GainedFocus,
				// Keyboard
				KeyPressed, KeyReleased,
				// Mouse
				MouseButtonPressed, MouseButtonReleased,
				MouseScrolled, MouseMoved,

				TypeCount,
				// Not specified
				Other
			} type;
		};
	}
}

#endif
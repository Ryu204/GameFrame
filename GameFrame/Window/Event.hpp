#ifndef __WINDOW_EVENT_HPP__
#define __WINDOW_EVENT_HPP__

/*
	Events will be used during window's input handling
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

			// Position of mouse after moving
			struct MousePosition
			{
				int x;
				int y;
			} mousePos;

			// Scroll delta of mouse
			float mouseScrollDelta; // Positive value means upwards movement

			// Mouse button clicked
			Mouse::Button button;

			// Keyboard key pressed
			Keyboard::Key key;

			// Event type
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
				// Not specified (Other irrelevant event)
				Other
			} type;
		};
	}
}

#endif
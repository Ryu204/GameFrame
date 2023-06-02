#ifndef GAMEFRAME_WINDOW_EVENT_HPP
#define GAMEFRAME_WINDOW_EVENT_HPP

/*
	Events will be used during window's input handling
*/

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace HJUIK::Window
{
	struct Event
	{
		struct Size
		{
			unsigned int Width;
			unsigned int Height;
		};
		struct MousePosition
		{
			int X;
			int Y;
		};
		union
		{
			// Size of the screen after resizing
			Size Resize;
			// Position of mouse
			MousePosition MousePos;
			// Delta of mouse wheel
			float MouseDelta;
			// Mouse button 
			Mouse::Button Button;
			// Keyboard key pressed
			Keyboard::Key Key;
		} Data;

		// Event type
		enum Type
		{
			// Window
			RESIZED = 0, CLOSED, LOST_FOCUS, GAINED_FOCUS,
			// Keyboard
			KEY_PRESSED, KEY_RELEASED,
			// Mouse
			MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED,
			MOUSE_SCROLLED, MOUSE_MOVED,

			TYPE_COUNT,
			// Not specified (Other irrelevant event)
			OTHER
		} Type;
	};
}

#endif
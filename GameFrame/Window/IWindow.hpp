#ifndef __WINDOW_WINDOW_HPP__
#define __WINDOW_WINDOW_HPP__

/*
	The abstract window class for specific windows 
	from different libraries
*/

#include "WindowType.hpp"
#include "Event.hpp"
#include "../Utilize/GLMTypedef.hpp"
#include "../Graphics/Color.hpp"

#include <string>

namespace HJUIK
{
	namespace Window
	{
		// Abstract window class
		class IWindow
		{
		// A derived class's constructor must be in this form:
		// DerivedClass(Vector2u size, std::string title, Style style = Style::Default);
		public:
			// Check if the window is still opened and available
			virtual bool isOpen() const = 0;
			// Get size of the window in pixels
			virtual Vector2u getSize() const = 0;
			// Close the window
			virtual void close() = 0;
			// This function polls the event from the queue and returns false if
			// nothing is found
			virtual bool pollEvent(Event& e) = 0;
			// Registers key pressed event only once after it is pressed if set 
			// to false, otherwise registers the event every frame it is held
			virtual void setKeyRepeatable(bool repeatable = true) = 0;
			// Try to match the framerate to FPS value, note that this function
			// is not precise
			virtual void limitFrameRate(unsigned int FPS) = 0;
			// Clear the screen of the window
			virtual void clear(Graphics::Color color = Graphics::Color(0, 0, 0)) = 0;
			// Display the content after finished rendering
			virtual void display() = 0;
		};
	}
}

#endif
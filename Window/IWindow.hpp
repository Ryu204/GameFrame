#ifndef __WINDOW_WINDOW_HPP__
#define __WINDOW_WINDOW_HPP__

/*
	Define the abstract window class for specific
	libraries' implementations
*/

#include "WindowType.hpp"
#include "../Utilize/GLMTypedef.hpp"

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
			virtual bool isOpen() const = 0;
			virtual Vector2u getSize() const = 0;
			virtual void close() = 0;
		};
	}
}

#endif
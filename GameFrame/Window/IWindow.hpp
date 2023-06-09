#ifndef GAMEFRAME_WINDOW_IWINDOW_HPP
#define GAMEFRAME_WINDOW_IWINDOW_HPP

/*
	The abstract window class for specific windows from different libraries
*/

#include <string>

#include "../Graphics/Color.hpp"
#include "../Graphics/IOpenGLContext.hpp"
#include "../Utilize/GLMTypedef.hpp"
#include "Event.hpp"
#include "WindowType.hpp"

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
			IWindow()								   = default;
			IWindow(const IWindow&)					   = default;
			IWindow(IWindow&&)						   = default;
			auto operator=(const IWindow&) -> IWindow& = default;
			auto operator=(IWindow&&) -> IWindow&	   = default;
			// Virtual destructor for use of std::unique_ptr
			virtual ~IWindow() = default;

			// Check if the window is still opened and available
			virtual auto isOpen() const -> bool = 0;
			// Get size of the window in pixels
			virtual auto getSize() const -> Vector2u = 0;
			// Close the window
			virtual auto close() -> void = 0;
			// This function polls the event from the queue and returns false if
			// nothing is found
			virtual auto pollEvent(Event& event) -> bool = 0;
			// Registers key pressed event only once after it is pressed if set
			// to false, otherwise registers the event every frame it is held
			virtual auto setKeyRepeatable(bool repeatable) -> void = 0;
			// Try to match the framerate to FPS value, note that this function
			// is not precise
			virtual auto limitFrameRate(unsigned int FPS) -> void = 0;
			// Get the OpenGL context that corresponds to this window
			virtual auto getOpenGLContext() -> Graphics::IOpenGLContext& = 0;
		};
	} // namespace Window
} // namespace HJUIK

#endif

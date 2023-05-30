#ifndef __WINDOW_SFMLWINDOW_HPP__
#define __WINDOW_SFMLWINDOW_HPP__

/*
	The derived window class with SFML implementation
*/

#include "IWindow.hpp"
#include "../Utilize/GLMTypedef.hpp"

#include "SFML/Window.hpp"

#include <memory>

namespace HJUIK
{
	namespace Window
	{
		// A SFML window capable of rendering and receiving input
		class SFMLWindow : public IWindow
		{
		public:
			// Window construction parameters
			struct Settings
			{
				Vector2u size;
				std::string title;
				WindowType style;
				unsigned int OpenGLMajorVersion = 3;
				unsigned int OpenGLMinorVersion = 3;
			};
		public:
			SFMLWindow(Vector2u size, std::string title, WindowType style = WindowType::Default);
			SFMLWindow(Settings settings);
			// Check if the window is still opened and available
			bool isOpen() const override;
			// Get size of the window in pixels
			Vector2u getSize() const override;
			// Close the window
			void close() override;
			// This function polls the event from the queue and returns false if
			// nothing is found
			bool pollEvent(Event& e) override;
			// Registers key pressed event only once after it is pressed if set 
			// to false, otherwise registers the event every frame it is held
			void setKeyRepeatable(bool repeatable = true) override;
			// Try to match the framerate to FPS value, note that this function
			// is not precise
			void limitFrameRate(unsigned int FPS) override;
			// Clear the screen of the window
			void clear(Graphics::Color color = Graphics::Color(0, 0, 0)) override;
			// Display the content after finished rendering
			void display() override;
		private:
			// Helper functions
			void createWindow(Settings settings);
			void loadOpenGL();
		private:
			std::unique_ptr<sf::Window> mWindow;
			// OpenGL status
			bool& mOpenGLLoaded() const;
		};
	}
}

#endif
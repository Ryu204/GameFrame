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
		class SFMLWindow : public IWindow
		{
		public:
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
			bool isOpen() const override;
			Vector2u getSize() const override;
			void close() override;
			bool pollEvent(Event& e) override;
			void setKeyRepeatable(bool repeatable = true) override;
			void limitFrameRate(unsigned int FPS) override;
			void display() override;
			void clear(Graphics::Color color = Graphics::Color(0, 0, 0)) override;
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
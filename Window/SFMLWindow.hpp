#ifndef __WINDOW_SFMLWINDOW_HPP__
#define __WINDOW_SFMLWINDOW_HPP__

/*
	The derived window class with SFML implementation
*/

#include "IWindow.hpp"
#include "../Utilize/GLMTypedef.hpp"

#include "SFML/Graphics.hpp"

#include <memory>

namespace HJUIK
{
	namespace Window
	{
		class SFMLWindow : public IWindow
		{
		public:
			SFMLWindow(Vector2u size, std::string title, WindowType style = WindowType::Default);
			bool isOpen() const override;
			Vector2u getSize() const override;
			void close() override;
			bool pollEvent(Event& e) override;
			void setKeyRepeatable(bool repeatable = true) override;
			void limitFrameRate(unsigned int FPS) override;
		private:
			std::unique_ptr<sf::RenderWindow> mWindow;
		};
	}
}

#endif
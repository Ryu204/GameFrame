#ifndef __WINDOW_SFMLWINDOW_HPP__
#define __WINDOW_SFMLWINDOW_HPP__

#include "IWindow.hpp"
#include "../Utilize/GLMTypedef.hpp"

#include "SFML/Graphics.hpp"

#include <memory>

namespace HJUIK
{
	namespace Window
	{
		class SFMLWindow : IWindow
		{
		public:
			SFMLWindow(Vector2u size, std::string title, WindowType style = WindowType::Default);
			bool isOpen() const override;
			Vector2u getSize() const override;
			void close() override;
		private:
			std::unique_ptr<sf::RenderWindow> mWindow;
		};
	}
}

#endif
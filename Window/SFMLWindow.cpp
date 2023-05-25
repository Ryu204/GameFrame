#include "SFMLWindow.hpp"

namespace HJUIK
{
	namespace Window
	{
		SFMLWindow::SFMLWindow(Vector2u size, std::string title, WindowType style)
			: mWindow()
		{
			auto windowStyle = sf::Style::Default;
			switch (style)
			{
			case WindowType::None:
				windowStyle = sf::Style::None;
				break;
			case WindowType::FullScreen:
				windowStyle = sf::Style::Fullscreen;
				break;
			case WindowType::Resizable:
				windowStyle = sf::Style::Default;
				break;
			case WindowType::Unresizable:
				windowStyle = sf::Style::Close;
				break;
			}

			mWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(size.x, size.y), title, windowStyle);
		}

		bool SFMLWindow::isOpen() const
		{
			return mWindow->isOpen();
		}

		Vector2u SFMLWindow::getSize() const
		{
			auto size = mWindow->getSize();
			return Vector2u(size.x, size.y);
		}

		void SFMLWindow::close()
		{
			mWindow->close();
		}
	}
}
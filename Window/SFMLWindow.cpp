#include "SFMLWindow.hpp"
#include <iostream>

namespace HJUIK
{
	namespace Window
	{
		SFMLWindow::SFMLWindow(Vector2u size, std::string title, WindowType style)
			: mWindow()
		{
			auto windowStyle = sf::Style::Default;
			auto videoMode = sf::VideoMode(size.x, size.y);
			switch (style)
			{
			case WindowType::None:
				windowStyle = sf::Style::None;
				break;
			case WindowType::FullScreen:
				windowStyle = sf::Style::Fullscreen;
				videoMode = sf::VideoMode::getDesktopMode();
				break;
			case WindowType::Resizable:
				windowStyle = sf::Style::Default;
				break;
			case WindowType::Unresizable:
				windowStyle = sf::Style::Close;
				break;
			}

			mWindow = std::make_unique<sf::RenderWindow>(videoMode, title, windowStyle);
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

		bool SFMLWindow::pollEvent(Event& e)
		{
			// We just pass the value from sf::Window::pollEvent(sf::Event& e)
			sf::Event event;
			if (mWindow->pollEvent(event) == false)
				return false;

			switch (event.type)
			{
			case sf::Event::Resized:
				e.type = Event::Type::Resized;
				e.size.x = event.size.width;
				e.size.y = event.size.height;
				break;
			case sf::Event::Closed:
				e.type = Event::Type::Closed;
				break;
			case sf::Event::GainedFocus:
				e.type = Event::Type::GainedFocus;
				break;
			case sf::Event::LostFocus:
				e.type = Event::Type::LostFocus;
				break;
			case sf::Event::KeyPressed:
				// std::cout << "KeyPressed!!\t";
				e.type = Event::Type::KeyPressed;
				e.key = Keyboard::fromSFMLKey(event.key.scancode);
				break;
			case sf::Event::KeyReleased:
				e.type = Event::Type::KeyReleased;
				e.key = Keyboard::fromSFMLKey(event.key.scancode);
				break;
			case sf::Event::MouseButtonPressed:
				e.type = Event::Type::MouseButtonPressed;
				e.button = Mouse::fromSFMLButton(event.mouseButton.button);
				break;
			case sf::Event::MouseButtonReleased:
				e.type = Event::Type::MouseButtonReleased;
				e.button = Mouse::fromSFMLButton(event.mouseButton.button);
				break;
			case sf::Event::MouseWheelScrolled:
				e.type = Event::Type::MouseScrolled;
				e.mouseScrollDelta = event.mouseWheelScroll.delta;
				break;
			default:
				e.type = Event::OTHER;
				break;
			}

			return true;
		}

		void SFMLWindow::setKeyRepeatable(bool repeatable)
		{
			mWindow->setKeyRepeatEnabled(repeatable);
		}

		void SFMLWindow::limitFrameRate(unsigned int FPS)
		{
			mWindow->setFramerateLimit(FPS);
		}
	}
}
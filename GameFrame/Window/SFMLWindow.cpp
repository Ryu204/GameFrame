#include "SFMLWindow.hpp"

#include "../Graphics/glad/glad.h"
#include <iostream>

namespace HJUIK
{
	namespace Window
	{
		auto SFMLWindow::mOpenGLLoaded() -> bool&
		{
			static bool loaded = false;
			return loaded;
		}

		auto SFMLWindow::loadOpenGL() -> void
		{
			if (!mOpenGLLoaded()) {
				gladLoadGL();
				mOpenGLLoaded() = true;
			}
		}

		SFMLWindow::SFMLWindow(const SFMLWindow::Settings& settings)
		{
			createWindow(settings);
			loadOpenGL();
		}

		SFMLWindow::SFMLWindow(Vector2u size, std::string title, WindowType style)
		{
			SFMLWindow::Settings const settings{size, std::move(title), style, 3, 3};
			createWindow(settings);
			loadOpenGL();
		}

		auto SFMLWindow::isOpen() const -> bool
		{
			return mWindow->isOpen();
		}

		auto SFMLWindow::getSize() const -> Vector2u
		{
			auto size = mWindow->getSize();
			return {size.x, size.y};
		}

		auto SFMLWindow::close() -> void
		{
			mWindow->close();
		}

		auto SFMLWindow::pollEvent(Event& event) -> bool
		{
			// We just pass the value from sf::Window::pollEvent(sf::Event& e)
			sf::Event sfEvent{};
			if (!mWindow->pollEvent(sfEvent)) {
				return false;
			}

			switch (sfEvent.type) {
			case sf::Event::Resized:
				event.getData() = EventType::Resize{sfEvent.size.width, sfEvent.size.height};
				break;
			case sf::Event::Closed:
				event.getData() = EventType::Close{};
				break;
			case sf::Event::GainedFocus:
				event.getData() = EventType::GainFocus{};
				break;
			case sf::Event::LostFocus:
				event.getData() = EventType::LoseFocus{};
				break;
			case sf::Event::KeyPressed:
				event.getData() = EventType::KeyPress{Keyboard::fromSFMLKey(sfEvent.key.scancode)};
				break;
			case sf::Event::KeyReleased:
				event.getData() = EventType::KeyRelease{Keyboard::fromSFMLKey(sfEvent.key.scancode)};
				break;
			case sf::Event::MouseButtonPressed:
				event.getData() = EventType::MousePress{Mouse::fromSFMLButton(sfEvent.mouseButton.button)};
				break;
			case sf::Event::MouseButtonReleased:
				event.getData() = EventType::MouseRelease{Mouse::fromSFMLButton(sfEvent.mouseButton.button)};
				break;
			case sf::Event::MouseWheelScrolled:
				event.getData() = EventType::MouseScroll{sfEvent.mouseWheelScroll.delta};
				break;
			case sf::Event::MouseMoved:
				event.getData() = EventType::MouseMove{sfEvent.mouseMove.x, sfEvent.mouseMove.y};
				break;
			default:
				event.getData() = EventType::Unknown{};
				break;
			}

			return true;
		}

		auto SFMLWindow::setKeyRepeatable(bool repeatable) -> void
		{
			mWindow->setKeyRepeatEnabled(repeatable);
		}

		auto SFMLWindow::limitFrameRate(unsigned int FPS) -> void
		{
			mWindow->setFramerateLimit(FPS);
		}

		auto SFMLWindow::display() -> void
		{
			mWindow->display();
		}

		auto SFMLWindow::clear(Graphics::Color color) -> void
		{
			auto normalized = color.getNormalizedColor();
			glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		auto SFMLWindow::createWindow(const SFMLWindow::Settings& settings) -> void
		{
			sf::ContextSettings csettings;
			csettings.sRgbCapable  = true;
			csettings.majorVersion = settings.OpenGLMajorVersion;
			csettings.minorVersion = settings.OpenGLMinorVersion;

			auto size  = settings.Size;
			auto title = settings.Title;
			auto style = settings.Style;

			auto windowStyle = sf::Style::Default;
			auto videoMode	 = sf::VideoMode(size.x, size.y);

			switch (style) {
			case WindowType::NONE:
				windowStyle = sf::Style::None;
				break;
			case WindowType::FULLSCREEN:
				windowStyle = sf::Style::Fullscreen;
				videoMode	= sf::VideoMode::getDesktopMode();
				break;
			case WindowType::RESIZABLE:
				windowStyle = sf::Style::Default;
				break;
			case WindowType::UNRESIZABLE:
				windowStyle = sf::Style::Close;
				break;
			}

			mWindow = std::make_unique<sf::Window>(videoMode, title, windowStyle, csettings);
		}
	} // namespace Window
} // namespace HJUIK
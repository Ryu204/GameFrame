#include <iostream>
#include "../Utilize/ResourceHolder.hpp"

struct Triple
{
	float x; float y; float z;

	Triple& operator = (Triple& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		std::cout << "Copied";
		return *this;
	}

	Triple(int a = 0, int b = 0, int c = 0)
	{
		x = a; y = b; z = c;
	}
};

#include "../Window/SFMLWindow.hpp"
#include "../Window/GLFWWindow.hpp"
#include "../Window/Keyboard.hpp"
#include "../Window/Mouse.hpp"

int main()
{
	using namespace HJUIK;
	std::unique_ptr<Window::IWindow> window = std::make_unique<Window::SFMLWindow>(Vector2u(500, 500), "hello world", Window::WindowType::Default);
	window->setKeyRepeatable(false);
	window->limitFrameRate(60);
	while (window->isOpen())
	{
		Event e;
		while (window->pollEvent(e))
		{
			if (e.type == Event::Type::Closed)
				window->close();
			else if (e.type == Event::Type::Resized)
				std::cout << e.size.x << ' ' << e.size.y << '\n';
			else if (e.type == Event::KeyPressed)
			{
				std::cout << Keyboard::getName(e.key) << '\n';
			}
			else if (e.type == Event::MouseButtonPressed)
				std::cout << Mouse::getName(e.button) << '\n';
			else if (e.type == Event::MouseScrolled)
				std::cout << e.mouseScrollDelta << '\n';
		}

		/*if (Keyboard::isPressed(Keyboard::Key::A))
			std::cout << "YES";*/
	}
}
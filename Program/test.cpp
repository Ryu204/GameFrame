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
#include "../Window/Keyboard.hpp"

int main()
{
	using namespace HJUIK;
	Window::SFMLWindow window(Vector2u(500, 500), "hello world", Window::WindowType::Default);

	while (window.isOpen())
	{
		if (Keyboard::isPressed(Keyboard::Key::P))
			std::cout << "P";
	}
}
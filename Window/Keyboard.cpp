#include "Keyboard.hpp"

namespace HJUIK
{
	namespace Keyboard
	{
		bool isPressed(Key key)
		{
			return sf::Keyboard::isKeyPressed(key);
		}
	}
}
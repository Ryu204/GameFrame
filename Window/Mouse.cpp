#include "Mouse.hpp"

namespace HJUIK
{
	namespace Mouse
	{
		bool isPressed(Button button)
		{
			return sf::Mouse::isButtonPressed(toSFMLButton(button));
		}

		sf::Mouse::Button toSFMLButton(Button button)
		{
			switch (button)
			{
			case Button::Left:
				return sf::Mouse::Left;
			case Button::Right:
				return sf::Mouse::Right;
			case Button::Middle:
				return sf::Mouse::Middle;
			case Button::Extra1:
				return sf::Mouse::XButton1;
			case Button::Extra2:
				return sf::Mouse::XButton2;
			case Button::ButtonCount:
				return sf::Mouse::ButtonCount;
			default:
				return sf::Mouse::Left;
			}
		}

		Button fromSFMLButton(sf::Mouse::Button button)
		{
			// The enum are originally from SFML so we only need to cast
			return static_cast<Button>(static_cast<std::int8_t>(button));
		}

		std::string getName(Button button)
		{
			switch (button)
			{
			case Button::Left:
				return "Left";
			case Button::Right:
				return "Right";
			case Button::Middle:
				return "Middle";
			case Button::Extra1:
				return "Extra Button 1";
			case Button::Extra2:
				return "Extra Button 2";
			default:
				return "Unknown";
			}
		}
	}
}
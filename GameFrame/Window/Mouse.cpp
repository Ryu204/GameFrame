#include "Mouse.hpp"

namespace HJUIK
{
	namespace Window
	{
		auto Mouse::isPressed(Button button) -> bool
		{
			return sf::Mouse::isButtonPressed(toSFMLButton(button));
		}

		auto Mouse::toSFMLButton(Button button) -> sf::Mouse::Button
		{
			switch (button)
			{
			case Button::LEFT:
				return sf::Mouse::Left;
			case Button::RIGHT:
				return sf::Mouse::Right;
			case Button::MIDDLE:
				return sf::Mouse::Middle;
			case Button::EXTRA_1:
				return sf::Mouse::XButton1;
			case Button::EXTRA_2:
				return sf::Mouse::XButton2;
			case Button::BUTTON_COUNT:
				return sf::Mouse::ButtonCount;
			default:
				return sf::Mouse::Left;
			}
		}

		auto Mouse::fromSFMLButton(sf::Mouse::Button button) -> Mouse::Button
		{
			// The enum are originally from SFML so we only need to cast
			return static_cast<Button>(static_cast<std::int8_t>(button));
		}

		auto Mouse::getName(Mouse::Button button) -> std::string
		{
			switch (button)
			{
			case Button::LEFT:
				return "Left";
			case Button::RIGHT:
				return "Right";
			case Button::MIDDLE:
				return "Middle";
			case Button::EXTRA_1:
				return "Extra Button 1";
			case Button::EXTRA_2:
				return "Extra Button 2";
			case Button::BUTTON_COUNT:
			case Button::UNKNOWN:
				return "Unknown";
			}
		}
	}
}
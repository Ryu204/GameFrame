#ifndef GAMEFRAME_WINDOW_MOUSE_HPP
#define GAMEFRAME_WINDOW_MOUSE_HPP

/*
    Define mouse buttons. Use SFML scancode in SFML/Window/Mouse.hpp.
*/

#include <string>
#include <cstdint>

#include "SFML/Window.hpp"

namespace HJUIK
{
    namespace Window
    {
        // Static class for querying real-time mouse information
        class Mouse
        {
        public:
            // From SFML include/Window/Event.hpp
            enum class Button : std::int8_t
            {
                UNKNOWN = -1,
                LEFT = 0, 
                RIGHT,
                MIDDLE,
                EXTRA_1,
                EXTRA_2,

                BUTTON_COUNT
            };
            
            // Get the name of the button
            static auto getName(Button button) -> std::string;
            // Return true if the button is currently pressed, and false otherwise
            static auto isPressed(Button button) -> bool;

            // Library specific functions
            static auto toSFMLButton(Button button) -> sf::Mouse::Button;
            static auto fromSFMLButton(sf::Mouse::Button button) -> Button;
        };
    } // namespace Window
} // namespace HJUIK

#endif

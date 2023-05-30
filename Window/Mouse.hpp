#ifndef __WINDOW_MOUSE_HPP__
#define __WINDOW_MOUSE_HPP__

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
                Unknown = -1,
                Left = 0, 
                Right,
                Middle,
                Extra1,
                Extra2,

                ButtonCount
            };
        public:
            // Get the name of the button
            static std::string getName(Button button);
            // Return true if the button is currently pressed, and false otherwise
            static bool isPressed(Button button);

            // Library specific functions
            static sf::Mouse::Button toSFMLButton(Button button);
            static Button fromSFMLButton(sf::Mouse::Button button);
        };
    }
}

#endif

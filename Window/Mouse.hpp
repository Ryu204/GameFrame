#ifndef __WINDOW_MOUSE_HPP__
#define __WINDOW_MOUSE_HPP__

/*
    Define mouse buttons. Used SFML's enums.
*/

#include <string>

#include "SFML/Window.hpp"

namespace HJUIK
{
    namespace Window
    {
        class Mouse
        {
        public:
            // From SFML include/Window/Event.hpp
            enum class Button : std::int8_t
            {
                Unknown = -1,
                Left = 0,     //!< The left mouse button
                Right,    //!< The right mouse button
                Middle,   //!< The middle (wheel) mouse button
                Extra1, //!< The first extra mouse button
                Extra2, //!< The second extra mouse button

                ButtonCount //!< Keep last -- the total number of mouse buttons
            };
        public:
            static std::string getName(Button button);
            static bool isPressed(Button button);
            static sf::Mouse::Button toSFMLButton(Button button);
            static Button fromSFMLButton(sf::Mouse::Button button);
        };
    }
}

#endif
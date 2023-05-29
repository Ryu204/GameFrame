#ifndef __PROGRAM_PROPERTIES_HPP__
#define __PROGRAM_PROPERTIES_HPP__

/*
	Define core properties of the application,
	such as FPS, window's size, ...
	Work similiar to a init file
*/

#include "../Utilize.hpp"
#include "../Window.hpp"

#include <string>

namespace HJUIK
{
	// Get the title of window
	std::string GET_WINDOW_TITLE();
	// Get window type
	WindowType GET_WINDOW_TYPE();
	// Get the interval of Application::update(Time) call
	Time GET_UPDATE_INTERVAL();
	// Get the size of starting window
	Vector2u GET_START_WINDOW_SIZE();
	// Get the starting framerate
	unsigned int GET_FRAMERATE();
	// True if the key events are repeatable when pressed
	bool GET_KEY_REPEATABILITY();
}

#endif
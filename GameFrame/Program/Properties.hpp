#ifndef GAMEFRAME_PROGRAM_PROPERTIES_HPP
#define GAMEFRAME_PROGRAM_PROPERTIES_HPP

/*
	Define core properties of the application,
	such as FPS, window's size, ...
	Work similiar to an init file
*/

#include "../Utilize.hpp"
#include "../Window.hpp"

#include <string>

namespace HJUIK
{
	// Get the title of window
	auto getWindowTitle() -> std::string;
	// Get window type
	auto getWindowType() -> WindowType;
	// Get the interval of Application::update(Time) call
	auto getUpdateInterval() -> Time;
	// Get the size of starting window
	auto getDefaultWindowSize() -> Vector2u;
	// Get the starting framerate
	auto getFramerate() -> unsigned int;
	// True if the key events are repeatable when pressed
	auto getKeyRepeatability() -> bool;
}

#endif
#ifndef GAMEFRAME_WINDOW_WINDOWTYPE_HPP
#define GAMEFRAME_WINDOW_WINDOWTYPE_HPP

/*
	Specify the attribute of a window
*/

namespace HJUIK::Window
{
	enum class WindowType
	{
		NONE = 0,
		RESIZABLE = 1 << 0,
		FULLSCREEN = 1 << 2,
		UNRESIZABLE = 1 << 3,
		DEFAULT = RESIZABLE
	};
}

#endif
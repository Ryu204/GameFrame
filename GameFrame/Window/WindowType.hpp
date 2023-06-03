#ifndef GAMEFRAME_WINDOW_WINDOWTYPE_HPP
#define GAMEFRAME_WINDOW_WINDOWTYPE_HPP

/*
	Specify the attribute of a window
*/

#include <cstdint>

namespace HJUIK
{
	namespace Window
	{
		enum class WindowType : std::uint8_t
		{
			NONE = 0,
			RESIZABLE = 1U << 0U,
			FULLSCREEN = 1U << 2U,
			UNRESIZABLE = 1U << 3U,
			DEFAULT = RESIZABLE
		};
	} // namespace Window
} // namespace HJUIK

#endif
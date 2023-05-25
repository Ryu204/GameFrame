#ifndef __WINDOW_STYLE_HPP__
#define __WINDOW_STYLE_HPP__

namespace HJUIK
{
	namespace Window
	{
		enum class WindowType
		{
			None = 0,
			Resizable = 1 << 0,
			FullScreen = 1 << 2,
			Unresizable = 1 << 3,
			Default = Resizable
		};
	}
}

#endif
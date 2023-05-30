#ifndef __GRAPHICS_COLOR_HPP__
#define __GRAPHICS_COLOR_HPP__

/*
	Color represented in 8-bit RGBA format
*/

#include "../Utilize/GLMTypedef.hpp"
#include <cstdint>

namespace HJUIK
{
	namespace Graphics
	{
		struct Color
		{
			using val = std::uint8_t;
			val r;
			val g;
			val b;
			val a;
			Color(Color& other) = default;
			Color(Color&& other) = default;
			Color(Vector4u code);
			Color(Vector3u code);
			Color(val red, val green, val blue, val alpha = 255);
			// Get the RGBA format of the color in [0,1] range
			Vector4f getNormalizedColor();
		};

		bool operator == (const Color& left, const Color& right);
		bool operator != (const Color& left, const Color& right);
	}
}

#endif
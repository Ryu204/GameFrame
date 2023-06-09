#ifndef GAMEFRAME_GRAPHICS_COLOR_HPP
#define GAMEFRAME_GRAPHICS_COLOR_HPP

/*
	Color represented in 8-bit RGBA format
*/

#include <cstdint>

#include "../Utilize/GLMTypedef.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		// Color represented in 8-bit RGBA format
		struct Color {
			using val = std::uint8_t;
			val Red;
			val Green;
			val Blue;
			val Alpha;

			Color();
			Color(const Color& other) = default;
			Color(Color&& other)	  = default;
			~Color()				  = default;

			auto operator=(const Color& other) -> Color& = default;
			auto operator=(Color&& other) -> Color&		 = default;
			explicit Color(Vector4u code);
			explicit Color(Vector3u code);
            // NOLINTNEXTLINE(*-easily-swappable-parameters)
            explicit Color(val red, val green, val blue, val alpha = UINT8_MAX);
            // For example: auto color = Color(0xABCDEFFF);
            explicit Color(std::uint32_t hexacode);
			// Get the RGBA format of the color in [0,1] range
			auto getNormalizedColor() const -> Vector4f;
		};

		auto operator==(const Color& left, const Color& right) -> bool;
		auto operator!=(const Color& left, const Color& right) -> bool;
	} // namespace Graphics
} // namespace HJUIK

#endif

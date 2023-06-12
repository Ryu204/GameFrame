#include "Color.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		Color::Color() : Red(0), Green(0), Blue(0), Alpha(UINT8_MAX) {}

		Color::Color(Vector4u code) : Red(code.r), Green(code.g), Blue(code.b), Alpha(code.a) {}

		Color::Color(Vector3u code) : Red(code.r), Green(code.g), Blue(code.b), Alpha(UINT8_MAX) {}

        // NOLINTNEXTLINE(*-easily-swappable-parameters)
        Color::Color(val red, val green, val blue, val alpha)
            : Red(red), Green(green), Blue(blue), Alpha(alpha)
        {
        }

        // Ease of use when you found a color code and don't want to convert
        // NOLINTBEGIN
        Color::Color(std::uint32_t hexacode)
            : Red(static_cast<val>((hexacode & 0xff000000) >> 24))
            , Green(static_cast<val>((hexacode & 0x00ff0000) >> 16))
            , Blue(static_cast<val>((hexacode & 0x0000ff00) >> 8))
            , Alpha(static_cast<val>(hexacode & 0x000000ff))
        {
        }
        // NOLINTEND

		auto Color::getNormalizedColor() const -> Vector4f
		{
			const Vector4f res(Red, Green, Blue, Alpha);
			return res / static_cast<float> (UINT8_MAX);
		}

		auto operator==(const Color& left, const Color& right) -> bool
		{
			return left.Alpha == right.Alpha && left.Red == right.Red && left.Green == right.Green
				&& left.Blue == right.Blue;
		}

		auto operator!=(const Color& left, const Color& right) -> bool
		{
			return !(left == right);
		}
	} // namespace Graphics
} // namespace HJUIK

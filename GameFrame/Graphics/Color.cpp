#include "Color.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		Color::Color()
			: Red(0)
			, Green(0)
			, Blue(0)
			, Alpha(UINT8_MAX)
		{ }
		
		Color::Color(Vector4u code)
			: Red(code.r)
			, Green(code.g)
			, Blue(code.b)
			, Alpha(code.a)
		{ }

		Color::Color(Vector3u code)
			: Red(code.r)
			, Green(code.g)
			, Blue(code.b)
			, Alpha(UINT8_MAX)
		{ }

		auto Color::getNormalizedColor() const -> Vector4f
		{
			const Vector4f res(Red, Green, Blue, Alpha);
			return res / static_cast<float>UINT8_MAX;
		}

		auto operator == (const Color& left, const Color& right) -> bool
		{
			return left.Alpha == right.Alpha && left.Red == right.Red
				&& left.Green == right.Green && left.Blue == right.Blue;
		}

		auto operator != (const Color& left, const Color& right) -> bool
		{
			return !(left == right);
		}
	} // namespace Graphics
} // namespace HJUIK
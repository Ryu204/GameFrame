#include "Color.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		Color::Color(Vector4u code)
			: r(code.r)
			, g(code.g)
			, b(code.b)
			, a(code.a)
		{ }

		Color::Color(Vector3u code)
			: r(code.r)
			, g(code.g)
			, b(code.b)
			, a(255)
		{ }

		Color::Color(val red, val green, val blue, val alpha)
			: r(red)
			, g(green)
			, b(blue)
			, a(alpha)
		{ }

		Vector4f Color::getNormalizedColor()
		{
			Vector4f res(r, g, b, a);
			return res / 255.f;
		}

		bool operator == (const Color& left, const Color& right)
		{
			return left.a == right.a && left.r == right.r
				&& left.g == right.g && left.b == right.b;
		}

		bool operator != (const Color& left, const Color& right)
		{
			return !(left == right);
		}
	}
}
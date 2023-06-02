#include "Properties.hpp"

namespace HJUIK
{
	auto getWindowTitle() -> std::string
	{
		return "Application";
	}

	auto getWindowType() -> WindowType
	{
		return WindowType::DEFAULT;
	}

	auto getUpdateInterval() -> Time
	{
		static constexpr float duration = 60.f;
		return {1.f / duration};
	}

	auto getDefaultWindowSize() -> Vector2u
	{
		static constexpr Vector2u size{800, 800};
		return size;
	}

	auto getFramerate() -> unsigned int
	{
		constexpr unsigned int fps{ 120 };
		return fps;
	}

	auto getKeyRepeatability() -> bool
	{
		return false;
	}
}
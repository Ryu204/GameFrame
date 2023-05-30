#include "Properties.hpp"

namespace HJUIK
{
	std::string GET_WINDOW_TITLE()
	{
		return "Application";
	}

	WindowType GET_WINDOW_TYPE()
	{
		return WindowType::Default;
	}

	Time GET_UPDATE_INTERVAL()
	{
		return Time(1.f / 60.f);
	}

	Vector2u GET_START_WINDOW_SIZE()
	{
		return Vector2u(800, 800);
	}

	unsigned int GET_FRAMERATE()
	{
		return 120;
	}

	bool GET_KEY_REPEATABILITY()
	{
		return false;
	}
}
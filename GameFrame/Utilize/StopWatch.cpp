#include "StopWatch.hpp"

namespace HJUIK::Utilize
{
	StopWatch::StopWatch()
		: mInitialization(std::chrono::steady_clock::now())
		, mStart(mInitialization)
	{ }

	auto StopWatch::restart() -> Time
	{
		const Point now = std::chrono::steady_clock::now();
		const std::chrono::duration<float> duration = (now - mStart);
		mStart = now;
		return { static_cast<float>(duration.count()) };
	}

	auto StopWatch::total() -> Time
	{
		return {static_cast<float>((std::chrono::steady_clock::now() - mInitialization).count())};
	}
}
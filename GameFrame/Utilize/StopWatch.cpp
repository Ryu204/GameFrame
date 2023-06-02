#include "StopWatch.hpp"

namespace HJUIK
{
	namespace Utilize
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
			return Time{ static_cast<float>(duration.count()) };
		}

		auto StopWatch::total() -> Time
		{
			return Time{ static_cast<float>((std::chrono::steady_clock::now() - mInitialization).count()) };
		}
	} // namespace Utilize
} // namespace HJUIK
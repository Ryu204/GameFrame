#include "StopWatch.hpp"

namespace HJUIK
{
	namespace Utilize
	{
		StopWatch::StopWatch()
			: mInitialization(std::chrono::steady_clock::now())
			, mStart(mInitialization)
		{ }

		Time StopWatch::restart()
		{
			Point now = std::chrono::steady_clock::now();
			std::chrono::duration<float> duration = (now - mStart);
			mStart = now;
			return Time(duration.count());
		}

		Time StopWatch::total()
		{
			return Time((std::chrono::steady_clock::now() - mInitialization).count());
		}
	}
}
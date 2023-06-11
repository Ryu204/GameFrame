#include "StopWatch.hpp"

namespace HJUIK
{
	namespace Utilize
	{
		StopWatch::StopWatch() : mInitialization(std::chrono::steady_clock::now()), mStart(mInitialization) {}

		auto StopWatch::restart() -> Time
		{
			const Point now								= std::chrono::steady_clock::now();
			const std::chrono::duration<float> duration = (now - mStart);
			mStart										= now;
			const auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
			return Time{seconds};
		}

		auto StopWatch::total() -> Time
		{
			const auto duration = std::chrono::steady_clock::now() - mInitialization;
			const auto seconds	= std::chrono::duration_cast<std::chrono::duration<float>>(duration).count();
			return Time{seconds};
		}
	} // namespace Utilize
} // namespace HJUIK

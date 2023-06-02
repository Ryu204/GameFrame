#ifndef GAMEFRAME_UTILIZE_STOPWATCH_HPP
#define GAMEFRAME_UTILIZE_STOPWATCH_HPP

/*
	Uses to know how much time has passed since
	last check.
*/

#include "Time.hpp"
#include <chrono>

namespace HJUIK::Utilize
{
	class StopWatch
	{
	public:
		StopWatch();
		// Get how much time has passed since last restart
		auto restart() -> Time;
		// Get how much time has passed since initialization
		auto total() -> Time;
	private:
		using Point = std::chrono::time_point<std::chrono::steady_clock>;
	private:
		Point mInitialization;
		Point mStart;
	};
}

#endif
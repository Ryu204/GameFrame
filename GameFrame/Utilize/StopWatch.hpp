#ifndef __UTILIZE_STOPWATCH_HPP__
#define __UTILIZE_STOPWATCH_HPP__

/*
	Uses to know how much time has passed since
	last check.
*/

#include "Time.hpp"
#include <chrono>

namespace HJUIK
{
	namespace Utilize
	{
		class StopWatch
		{
		public:
			StopWatch();
			// Get how much time has passed since last restart
			Time restart();
			// Get how much time has passed since initialization
			Time total();
		private:
			using Point = std::chrono::time_point<std::chrono::steady_clock>;
		private:
			Point mInitialization;
			Point mStart;
		};
	}
}

#endif
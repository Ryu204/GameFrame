#ifndef __UTILIZE_TIME_HPP__
#define __UTILIZE_TIME_HPP__

#include <chrono>

namespace HJUIK
{
	namespace Utilize
	{
		class Time
		{
		public:
			Time(float second = 0.f);
			Time(const Time& other);
			float toSecond() const;

			Time& operator = (Time&) = default;
			Time& operator = (Time&&) = default;
			void operator += (const Time& time);
			void operator -= (const Time& time);
			void operator *= (float scalar);
			void operator /= (float scalar);
		private:
			float mSecond;
		};

		Time operator * (float scalar, const Time& time);
		Time operator * (const Time& time, float scalar);
		Time operator / (const Time& time, float scalar);
		Time operator + (const Time& left, const Time& right);
		Time operator - (const Time& left, const Time& right);
		Time operator - (const Time& left);
		bool operator == (const Time& left, const Time& right);
		bool operator != (const Time& left, const Time& right);
		bool operator < (const Time& left, const Time& right);
		bool operator > (const Time& left, const Time& right);
		bool operator >= (const Time& left, const Time& right);
		bool operator <= (const Time& left, const Time& right);
	}
}

#endif
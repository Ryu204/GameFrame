#include "Time.hpp"

namespace HJUIK
{
	namespace Utilize
	{
		Time::Time(float second)
			: mSecond(second)
		{ }

		Time::Time(const Time& other)
			: mSecond(other.mSecond)
		{ }

		float Time::toSecond() const
		{
			return mSecond;
		}

		void Time::operator += (const Time& time)
		{
			mSecond += time.mSecond;
		}

		void Time::operator -= (const Time& time)
		{
			mSecond -= time.mSecond;
		}

		void Time::operator *= (float scalar)
		{
			mSecond *= scalar;
		}

		void Time::operator /= (float scalar)
		{
			mSecond /= scalar;
		}

		Time operator * (float scalar, const Time& time)
		{
			return Time(scalar * time.toSecond());
		}

		Time operator * (const Time& time, float scalar)
		{
			return Time(scalar * time.toSecond());
		}

		Time operator / (const Time& time, float scalar)
		{
			return Time(time.toSecond() / scalar);
		}

		Time operator + (const Time& left, const Time& right)
		{
			return Time(left.toSecond() + right.toSecond());
		}

		Time operator - (const Time& left, const Time& right)
		{
			return Time(left.toSecond() - right.toSecond());
		}

		Time operator - (const Time& left)
		{
			return Time(-left.toSecond());
		}

		bool operator == (const Time& left, const Time& right)
		{
			return left.toSecond() == right.toSecond();
		}

		bool operator != (const Time& left, const Time& right)
		{
			return left.toSecond() != right.toSecond();
		}

		bool operator < (const Time& left, const Time& right)
		{
			return left.toSecond() < right.toSecond();
		}

		bool operator > (const Time& left, const Time& right)
		{
			return left.toSecond() > right.toSecond();
		}

		bool operator >= (const Time& left, const Time& right)
		{
			return left.toSecond() >= right.toSecond();
		}

		bool operator <= (const Time& left, const Time& right)
		{
			return left.toSecond() <= right.toSecond();
		}
	}
}
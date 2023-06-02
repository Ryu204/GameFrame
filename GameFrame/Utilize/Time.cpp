#include "Time.hpp"

namespace HJUIK::Utilize
{
	Time::Time(float second)
		: mSecond(second)
	{ }

	auto Time::toSecond() const -> float
	{
		return mSecond;
	}

	auto Time::operator += (const Time& time) -> void
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

	auto operator * (float scalar, const Time& time) -> Time
	{
		return { scalar * time.toSecond() };
	}

	auto operator * (const Time& time, float scalar) -> Time
	{
		return { scalar * time.toSecond() };
	}

	auto operator / (const Time& time, float scalar) -> Time
	{
		return { time.toSecond() / scalar };
	}

	auto operator + (const Time& left, const Time& right) -> Time
	{
		return { left.toSecond() + right.toSecond() };
	}

	auto operator - (const Time& left, const Time& right) -> Time
	{
		return { left.toSecond() - right.toSecond() };
	}

	auto operator - (const Time& left) -> Time
	{
		return { -left.toSecond() };
	}

	auto operator == (const Time& left, const Time& right) -> bool
	{
		return left.toSecond() == right.toSecond();
	}

	auto operator != (const Time& left, const Time& right) -> bool
	{
		return left.toSecond() != right.toSecond();
	}

	auto operator < (const Time& left, const Time& right) -> bool
	{
		return left.toSecond() < right.toSecond();
	}

	auto operator > (const Time& left, const Time& right) -> bool
	{
		return left.toSecond() > right.toSecond();
	}

	auto operator >= (const Time& left, const Time& right) -> bool
	{
		return left.toSecond() >= right.toSecond();
	}

	auto operator <= (const Time& left, const Time& right) -> bool
	{
		return left.toSecond() <= right.toSecond();
	}
}
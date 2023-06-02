#ifndef GAMEFRAME_UTILIZE_TIME_HPP
#define GAMEFRAME_UTILIZE_TIME_HPP

/*
	Represent time duration. Use std::chrono::steady_clock
*/

#include <chrono>

namespace HJUIK::Utilize
{
	class Time
	{
	public:
		Time(float second = 0.f);
		Time(const Time& other) = default;

		// Convert the time to float second value
		auto toSecond() const -> float;

		// Overload operators
		auto operator = (Time&) -> Time& = default;
		auto operator = (Time&&) -> Time& = default;
		auto operator += (const Time& time) -> void;
		auto operator -= (const Time& time) -> void;
		auto operator *= (float scalar) -> void;
		auto operator /= (float scalar) -> void;
	private:
		float mSecond;
	};

	// Overload operators
	auto operator * (float scalar, const Time& time) -> Time;
	auto operator * (const Time& time, float scalar) -> Time;
	auto operator / (const Time& time, float scalar) -> Time;
	auto operator + (const Time& left, const Time& right) -> Time;
	auto operator - (const Time& left, const Time& right) -> Time;
	auto operator - (const Time& left) -> Time;
	auto operator == (const Time& left, const Time& right) -> bool;
	auto operator != (const Time& left, const Time& right) -> bool;
	auto operator < (const Time& left, const Time& right) -> bool;
	auto operator > (const Time& left, const Time& right) -> bool;
	auto operator >= (const Time& left, const Time& right) -> bool;
	auto operator <= (const Time& left, const Time& right) -> bool;
}

#endif
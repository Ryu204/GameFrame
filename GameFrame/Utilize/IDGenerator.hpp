#ifndef GAMEFRAME_UTILIZE_ID_GENERATOR_HPP
#define GAMEFRAME_UTILIZE_ID_GENERATOR_HPP

#include <array>
#include <queue>
#include <exception>

#include "CallAssert.hpp"

namespace HJUIK
{
	namespace Utilize
	{
		// Manage ID creation and deletion
		// N is the number of maximum available IDs
		template <std::size_t N>
		class IDGenerator
		{
		public:
			IDGenerator();
			// Generate a new ID. Return 0 if all possible ID were generated and not erased.
			auto generate() -> std::size_t;
			// Erase an ID generated before
			auto erase(std::size_t ID) -> void; // NOLINT
			// Check if ID is still in active state
			auto isActive(std::size_t ID) -> bool; // NOLINT

		private:
            auto checkInRange(std::size_t ID) -> void; // NOLINT

			std::queue<std::size_t> mAvailable;
			std::array<bool, N> mAliveLists;
		};

		template <std::size_t N>
		IDGenerator<N>::IDGenerator()
		{
			for (std::size_t i = 0; i < N; ++i) {
				mAvailable.push(i);
				mAliveLists[i] = false;
			}
		}

		template <std::size_t N>
		auto IDGenerator<N>::generate() -> std::size_t
		{
            checkNonZero(!mAvailable.empty(), "HJUIK::IDGenerator: Max IDs number exceeded (" + std::to_string(N) + ")");

            const auto res = mAvailable.front();
            mAvailable.pop();
            mAliveLists[res] = true;
            return res;
		}

		template <std::size_t N>
		auto IDGenerator<N>::erase(std::size_t ID) -> void // NOLINT
		{
            checkInRange(ID);
            checkNonZero(mAliveLists[ID], "HJUIK::IDGenerator: Erase non-existed ID (" + std::to_string(ID) + ")");
			mAliveLists[ID] = false;
			mAvailable.push(ID);
		}

		template <std::size_t N>
		auto IDGenerator<N>::isActive(std::size_t ID) -> bool // NOLINT
		{
            checkInRange(ID);
			return mAliveLists[ID];
		}

        template <std::size_t N>
        auto IDGenerator<N>::checkInRange(std::size_t ID) -> void // NOLINT
        {
            checkNonZero(ID < N, "HJUIK::IDGenerator: ID query not in range (" + std::to_string(ID) + ")");
        }
	} // namespace Utilize
} // namespace HJUIK

#endif

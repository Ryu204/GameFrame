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
        // This class asserts excessively in debug build if the parameters are faulty
		template <std::size_t N>
		class IDGenerator
		{
		public:
			IDGenerator();
			// Generate a new ID.
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
            HJUIK_ASSERT(!mAvailable.empty(), "Max IDs number exceeded (", N, ")");

            const auto res = mAvailable.front();
            mAvailable.pop();
            mAliveLists[res] = true;
            return res;
		}

		template <std::size_t N>
		auto IDGenerator<N>::erase(std::size_t ID) -> void // NOLINT
		{
            checkInRange(ID);
            HJUIK_ASSERT(mAliveLists[ID], "Erase non-existed ID (", ID, ")");
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
            HJUIK_ASSERT(ID < N, "ID query not in range (", ID, ")");
        }
	} // namespace Utilize
} // namespace HJUIK

#endif

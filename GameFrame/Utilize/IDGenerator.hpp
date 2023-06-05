#ifndef GAMEFRAME_UTILIZE_ID_GENERATOR_HPP
#define GAMEFRAME_UTILIZE_ID_GENERATOR_HPP

#include <queue>
#include <array>

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
            std::queue<std::size_t> mAvailable;
            std::array<bool, N> mAliveLists;
        };

        template <std::size_t N>
        IDGenerator<N>::IDGenerator()
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                mAvailable.push(i);
                mAliveLists.at(i) = false;
            }
        }

        template <std::size_t N>
        auto IDGenerator<N>::generate() -> std::size_t
        {
            if (!mAvailable.empty())
            {
                std::size_t res = mAvailable.front();
                mAvailable.pop();
                mAliveLists[res] = true;
                return res;
            }

            return 0;
        }

        template <std::size_t N>
        auto IDGenerator<N>::erase(std::size_t ID) -> void // NOLINT
        {
            mAliveLists.at(ID) = false;
            mAvailable.push(ID);
        }

        template <std::size_t N>
        auto IDGenerator<N>::isActive(std::size_t ID) -> bool // NOLINT
        {
            return mAliveLists[ID];
        }
    } // namespace Utilize
} // namespace HJUIK

#endif
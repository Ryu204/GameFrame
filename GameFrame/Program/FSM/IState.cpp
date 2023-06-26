#include "IState.hpp"
#include "Vector.hpp"

namespace HJUIK
{
    namespace FSM
    {
        auto IState::requestPush(const ID& identifier) -> void
        {
            mVector->getRequest(Request::Push{identifier});
        }

        auto IState::requestErase(int index) -> void
        {
			mVector->getRequest(Request::EraseIndex{index});
		}

		auto IState::requestErase(const ID& identifier, bool onlyLast)
        {
			mVector->getRequest(Request::EraseIdentifier{identifier,
                onlyLast ? Request::EraseIdentifier::LAST : Request::EraseIdentifier::ALL});
		}

		auto IState::requestSelfPop() -> void
        {
            mVector->getRequest(Request::ErasePtr{this});
        }

        auto IState::requestClear() -> void
        {
            mVector->getRequest(Request::Clear{});
        }
    } // namespace FSM
} // namespace HJUIK
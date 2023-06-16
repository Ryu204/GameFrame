#include "FSMState.hpp"
#include "FSMStack.hpp"

namespace HJUIK
{
    namespace FSM
    {
        auto IState::requestStackPush(const ID& identifier) -> void
        {
            mStack->getRequest(detail::Request(detail::PushCall{identifier}));
        }

        auto IState::requestStackPop() -> void
        {
            mStack->getRequest(detail::Request(detail::PopCall()));
        }

        auto IState::requestStackClear() -> void
        {
            mStack->getRequest(detail::Request(detail::ClearCall()));
        }
    } // namespace FSM
} // namespace HJUIK

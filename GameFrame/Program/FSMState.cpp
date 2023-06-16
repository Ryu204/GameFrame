#include "FSMState.hpp"
#include "FSMStack.hpp"

namespace HJUIK
{
    namespace FSM
    {
        auto IState::requestStackPush(ID id) -> void
        {
            mStack->getRequest(detail::Request(detail::PushCall{id}));
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

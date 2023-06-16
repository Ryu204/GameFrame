#include "FSMStack.hpp"
#include "../Utilize/CallAssert.hpp"

namespace HJUIK
{
    namespace FSM
    {
        auto StateStack::getRequest(const detail::Request& request) -> void
        {
            mPendingChanges.push_back(request);
        }

        auto StateStack::flushRequests() -> void
        {
            static const auto visitor = detail::Visitor{
                // Push a new state
                [this](const detail::PushCall& call) {
                    HJUIK_ASSERT(mRegistered.find(call.ID) != mRegistered.end(), "Unregistered FSM state");
                    mStack.push_back(mRegistered[call.ID]());
                },
                // Pop the top state
                [this](const detail::PopCall&) {
                    HJUIK_ASSERT(!mStack.empty(), "StateStack is currently empty");
                    mStack.pop_back();
                },
                // Clear the stack
                [this](const detail::ClearCall&) {
                    mStack.clear();
                }
            };
            for (const auto& req : mPendingChanges)
            {
                std::visit(visitor, req);
            }
            mPendingChanges.clear();
        }

        auto StateStack::update(Utilize::Time deltaTime) -> void
        {
            for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++)
            {
                // If the current state blocks update of lower state
                if (!(*itr)->update(deltaTime))
                {
                    break;
                }
            }
            flushRequests();
        }

        auto StateStack::processInput(const Event& event) -> void
        {
            for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++)
            {
                // If the current state blocks events for lower state
                if (!(*itr)->processInput(event))
                {
                    break;
                }
            }
            flushRequests();
        }

        auto StateStack::render(Graphics::IOpenGLContext& context) -> void
        {
            for (auto itr = mStack.rbegin(); itr != mStack.rend(); itr++)
            {
                // If the current state is the last state that should be drawn
                if (!(*itr)->render(context))
                {
                    break;
                }
            }
            flushRequests();
        }
    } // namespace FSM
} // namespace HJUIK
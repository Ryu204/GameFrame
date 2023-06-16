#ifndef GAMEFRAME_PROGRAM_FSM_STACK_HPP
#define GAMEFRAME_PROGRAM_FSM_STACK_HPP

#include "FSMState.hpp"
#include <stack>
#include <queue>
#include <variant>
#include <unordered_map>
#include <functional>

namespace HJUIK
{
    namespace FSM
    {
        namespace detail
        {
            // Events to request push
            struct PushCall
            {
                IState::ID ID;
            };
            struct PopCall {};
            struct ClearCall {};
            using Request = std::variant<PushCall, PopCall, ClearCall>;
            
            // https://en.cppreference.com/w/cpp/utility/variant/visit
            template <typename... Fs>
            struct Visitor : Fs... {using Fs::operator ()...;};
            template <typename... Fs>
            Visitor(Fs...) -> Visitor<Fs...>;
        } // namespace detail

        class StateStack
        {
        public:
            using Ptr = std::unique_ptr<IState>;
            using CreatorFn = std::function<Ptr()>;
            StateStack() = default;
            auto getRequest(const detail::Request& request) -> void;
            auto update(Utilize::Time deltaTime) -> void;
            auto processInput(const Event& event) -> void;
            auto render(Graphics::IOpenGLContext& context) -> void;

        private:            
            auto flushRequests() -> void;
            std::vector<detail::Request> mPendingChanges;
            std::vector<Ptr> mStack;
            std::unordered_map<IState::ID, CreatorFn> mRegistered;
        };
    } // namespace FSM
} // namespace HJUIK

#endif
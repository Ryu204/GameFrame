#ifndef GAMEFRAME_PROGRAM_FSM_STATE_HPP
#define GAMEFRAME_PROGRAM_FSM_STATE_HPP

#include "../Utilize/Time.hpp"
#include "../Window/Event.hpp"
#include "../Graphics/IOpenGLContext.hpp"

namespace HJUIK
{
    namespace FSM
    {
        class StateStack;

        class IState
        {
        public:
            using ID = std::string;

            constexpr IState(StateStack* stack) : mStack{stack} {}
            virtual ~IState() = default;
            virtual auto update(Utilize::Time& deltaTime) -> bool {return false;}
            virtual auto processInput(const Event& event) -> bool {return false;}
            virtual auto render(Graphics::IOpenGLContext& context) -> bool {return false;}

            auto requestStackPush(ID id) -> void;
            auto requestStackPop() -> void;
            auto requestStackClear() -> void;
        private:
            StateStack* mStack;
        };
    } // namespace FSM
} // namespace HJUIK

#endif
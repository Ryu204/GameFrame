#ifndef GAMEFRAME_PROGRAM_FSM_STATE_HPP
#define GAMEFRAME_PROGRAM_FSM_STATE_HPP

/*
    Base class for game state. Each state is a part of the game that do
    and present different things. For example: Settings State, Menu State,
    GameOver State, Pause State, InGame State, etc.

    The return type of each method is `bool`. At each operation of the vector
    (i.e update, process inputs and render), if the processed state returns false,
    the vector will stop processing, i.e some of the last states get to update,
    render and process events.
*/

#include "../Utilize/Time.hpp"
#include "../Window/Event.hpp"
#include "../Graphics/IOpenGLContext.hpp"

namespace HJUIK
{
    namespace FSM
    {
        class StateVector;

        class IState
        {
        public:
            using ID = std::string;

            constexpr explicit IState(StateVector* vector) : mVector{vector} {}
			IState(const IState&) = delete;
			IState(IState&&)	  = delete;
			auto operator=(IState&&) -> IState& = delete;
			auto operator=(const IState&) -> IState& = delete;
			virtual ~IState()						 = default;
			virtual auto update(Utilize::Time) -> bool {return false;}
            virtual auto processInput(const Event&) -> bool {return false;}
            virtual auto render(Graphics::IOpenGLContext&) -> bool {return false;}

            // Pend change to the vector
            auto requestPush(const ID& identifier) -> void;
            auto requestErase(int index) -> void;
			auto requestErase(const ID& identifier, bool onlyLast = false);
			auto requestSelfPop() -> void;
			auto requestClear() -> void;

		private:
			StateVector* mVector;
        };
    } // namespace FSM
} // namespace HJUIK

#endif
#ifndef GAMEFRAME_PROGRAM_STATE_PAUSE_STATE_HPP
#define GAMEFRAME_PROGRAM_STATE_PAUSE_STATE_HPP

/*
    Mock up state derived class. FSM needs at least one available state run.
*/

#include "../FSM/IState.hpp"
#include "../../Window/Keyboard.hpp"
#include "../../Window/EventManager.hpp"
#include <string>
#include <iostream>

namespace HJUIK
{
    namespace FSM
    {
        class PauseState : public IState
        {
        public:
			explicit PauseState(StateVector* vector, int order)
            : IState(vector)
            , mOrder(order)
            {
				std::cout << "Pause state created!\n";
				initEventCallback();
			}
			PauseState(const PauseState&) = delete;
			PauseState(PauseState&&)		= delete;
			auto operator=(PauseState&&) -> PauseState& = delete;
			auto operator=(const PauseState&) -> PauseState& = delete;

			~PauseState() override
            {
				std::cout << "Pause state instance deleted\n";
			}

			auto initEventCallback() -> void
            {
                mEventManager.registerHandler<EventType::KeyPress>(
                    [this](const EventType::KeyPress& keyData){
                        if (keyData.Code == Window::Keyboard::Key::ESCAPE)
                        {
							requestSelfPop();
						}
                    }
                );
            }

			auto processInput(const Event& event) -> bool override
            {
				mEventManager.processEvent(event);
				return false;
			}

		private:
			int mOrder;
			EventManager mEventManager;
		};
	} // namespace FSM
} // namespace HJUIK

#endif

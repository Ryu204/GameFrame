#ifndef GAMEFRAME_PROGRAM_STATE_GAME_STATE_HPP
#define GAMEFRAME_PROGRAM_STATE_GAME_STATE_HPP

/*
    Mock up state derived class. FSM needs at least one available state run.
*/

#include "../FSM/State.hpp"
#include "../../Window/Keyboard.hpp"
#include "../../Window/EventManager.hpp"
#include <string>
#include <iostream>

namespace HJUIK
{
    namespace FSM
    {
        class GameState : public IState
        {
        public:
			explicit GameState(StateVector* vector, int order, std::string name)
            : IState(vector)
            , mOrder(order)
            , mContent(std::move(name))
            {
				std::cout << "Game state created!\n";
				initEventCallback();
			}
			GameState(const GameState&) = delete;
			GameState(GameState&&)		= delete;
			auto operator=(GameState&&) -> GameState& = delete;
			auto operator=(const GameState&) -> GameState& = delete;

			~GameState() override
            {
				std::cout << "Game state instance deleted\n";
			}

			auto initEventCallback() -> void
            {
                mEventManager.registerHandler<EventType::KeyPress>(
                    [this](const EventType::KeyPress& keyData){
                        if (keyData.Code == Window::Keyboard::Key::SLASH)
                        {
							std::cout << mContent << '\n';
						}
                        else if (keyData.Code == Window::Keyboard::Key::ENTER
                            || keyData.Code == Window::Keyboard::Key::NUMPAD_ENTER)
                        {
							std::cout << "You will be directed to a state with ID \"Menu 2\"\n";
							requestSelfPop();		  // Pop this state first
							requestPush("Menu 2"); // Switch to Game
						}
                        else if (keyData.Code == Window::Keyboard::Key::P)
                        {
                            // No pop because pause state is an overlay
							requestPush("Pause");
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
			std::string mContent;
			EventManager mEventManager;
		};
	} // namespace FSM
} // namespace HJUIK

#endif

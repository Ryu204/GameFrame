#ifndef GAMEFRAME_PROGRAM_STATE_MENU_STATE_HPP
#define GAMEFRAME_PROGRAM_STATE_MENU_STATE_HPP

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
        class MenuState : public IState
        {
        public:
			explicit MenuState(StateVector* vector, int order, std::string name)
            : IState(vector)
            , mOrder(order)
            , mContent(std::move(name))
            {
				std::cout << "Menu State created!\n";
				initEventCallback();
			}
			MenuState(const MenuState&) = delete;
			MenuState(MenuState&&)		= delete;
			auto operator=(MenuState&&) -> MenuState& = delete;
			auto operator=(const MenuState&) -> MenuState& = delete;

			~MenuState() override
            {
				std::cout << "Menu state instance deleted\n";
			}

			auto initEventCallback() -> void
            {
                mEventManager.registerHandler<EventType::KeyPress>(
                    [this](const EventType::KeyPress& keyData){
                        if (keyData.Code == Window::Keyboard::Key::Q)
                        {
							std::cout << mContent << '\n';
						}
                        else if (keyData.Code == Window::Keyboard::Key::S)
                        {
							requestErase(-1); // Pop this state first
							requestPush("Game"); // Switch to Game
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

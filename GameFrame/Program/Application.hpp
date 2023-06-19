#ifndef GAMEFRAME_PROGRAM_APPLICATION_HPP
#define GAMEFRAME_PROGRAM_APPLICATION_HPP

/*
	Application is the highest layer of wrapper and will be used in main() function.
*/

#include <memory>

#include "../Utilize.hpp"
#include "../Window.hpp"
#include "FSMVector.hpp"

namespace HJUIK
{
	class Application
	{
	public:
		Application();
		// Runs the application, returns only if the application is shutdown or terminates
		auto run() -> void;

	private:
		auto update(Time deltaTime) -> void;
		auto processInput() -> void;
		auto render() -> void;
		auto initEventCallback() -> void;
		auto initStateMachine() -> void;

		std::unique_ptr<IWindow> mWindow;
		EventManager mEventManager;
		const Time mUpdateInterval;

		FSM::StateVector mStateVector;
	};
} // namespace HJUIK

#endif
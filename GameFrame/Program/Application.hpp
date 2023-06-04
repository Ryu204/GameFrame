#ifndef GAMEFRAME_PROGRAM_APPLICATION_HPP
#define GAMEFRAME_PROGRAM_APPLICATION_HPP

/*
	Application class is the highest layer of wrapper and will be used in main() function.
*/

#include "../Window.hpp"
#include "../Utilize.hpp"

#include <memory>

namespace HJUIK
{
	class Application
	{
	public:
		Application();
		// Runs the application, returns only if the application
		// is shutdown or terminates
		auto run() -> void;
	private:
		auto update(Time deltaTime) -> void;
		auto processInput() -> void;
		auto render() -> void;

		auto initEventCallback() -> void;

		std::unique_ptr<IWindow> mWindow;
		EventManager mEventManager;
		const Time mUpdateInterval;
		Graphics::Color mBackgroundColor;
	};
} // namespace HJUIK

#endif
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
		void run();
	private:
		void update(Time deltaTime);
		void processInput();
		void render();


		std::unique_ptr<IWindow> mWindow;
		const Time mUpdateInterval;
		Graphics::Color mBackgroundColor;
	};
} // namespace HJUIK

#endif
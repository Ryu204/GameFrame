#ifndef GAMEFRAME_PROGRAM_APPLICATION_HPP
#define GAMEFRAME_PROGRAM_APPLICATION_HPP

/*
	The highest layer of the engine wrapper. 
	Will be used in main() function.
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
		void update(Time time);
		void processInput();
		void render();
	private:
		std::unique_ptr<IWindow> mWindow;

		const Time mUpdateInterval;
	};
}

#endif
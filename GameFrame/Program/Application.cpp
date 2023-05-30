#include "Application.hpp"
#include "Properties.hpp"
#include "../Graphics/Color.hpp"

namespace HJUIK
{
	Application::Application()
		: mUpdateInterval(GET_UPDATE_INTERVAL())
	{
		mWindow = std::make_unique<SFMLWindow>(
			GET_START_WINDOW_SIZE(),
			GET_WINDOW_TITLE(),
			GET_WINDOW_TYPE()
		);

		mWindow->limitFrameRate(GET_FRAMERATE());
		mWindow->setKeyRepeatable(GET_KEY_REPEATABILITY());
	}

	void Application::run()
	{
		Clock clock;
		Time elapsed = clock.restart();

		while (mWindow->isOpen())
		{
			elapsed += clock.restart();
			while (elapsed >= mUpdateInterval)
			{
				elapsed -= mUpdateInterval;
				processInput(mUpdateInterval);
				update(mUpdateInterval);
			}

			render();
		}
	}

	void Application::update(Time dt)
	{

	}

	void Application::processInput(Time dt)
	{
		Event e;
		while (mWindow->pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
				mWindow->close();
				break;
			default:
				break;
			}
		}
	}

	void Application::render()
	{
		mWindow->clear(Graphics::Color(10, 20, 30, 255));

		mWindow->display();
	}
}
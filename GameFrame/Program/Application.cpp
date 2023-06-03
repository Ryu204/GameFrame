#include "Application.hpp"
#include "Properties.hpp"
#include "../Graphics/Color.hpp"
#include <iostream>

namespace HJUIK
{
	Application::Application()
			: mUpdateInterval(getUpdateInterval())
	{
		mWindow = std::make_unique<SFMLWindow>(
				getDefaultWindowSize(),
				getWindowTitle(),
				getWindowType());

		mWindow->limitFrameRate(getFramerate());
		mWindow->setKeyRepeatable(getKeyRepeatability());
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
				processInput();
				update(mUpdateInterval);
			}

			render();
		}
	}

	void Application::update(Time deltaTime)
	{
	}

	void Application::processInput()
	{
		Event event;
		while (mWindow->pollEvent(event))
		{
			switch (event.Type)
			{
			case Event::CLOSED:
				mWindow->close();
				break;
			default:
				break;
			}
		}
	}

	void Application::render()
	{
		auto &glContext = mWindow->getOpenGLContext();
		glContext.clear(mBackgroundColor);
		glContext.display();
	}
} // namespace HJUIK

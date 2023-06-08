#include "Application.hpp"

#include "../Graphics/Color.hpp"
#include "Properties.hpp"

namespace HJUIK
{
	Application::Application() : mUpdateInterval(getUpdateInterval())
	{
		mWindow = std::make_unique<SFMLWindow>(getDefaultWindowSize(), getWindowTitle(), getWindowType());
		mWindow->limitFrameRate(getFramerate());
		mWindow->setKeyRepeatable(getKeyRepeatability());

		initEventCallback();
	}

	auto Application::run() -> void
	{
		Clock clock;
		Time elapsed = clock.restart();

		while (mWindow->isOpen()) {
			elapsed += clock.restart();
			while (elapsed >= mUpdateInterval) {
				elapsed -= mUpdateInterval;
				processInput();
				update(mUpdateInterval);
			}

			render();
		}
	}

	auto Application::update(Time deltaTime) -> void {}


	auto Application::processInput() -> void
	{
		Event event;
		while (mWindow->pollEvent(event)) {
			mEventManager.processEvent(event);
		}
	}

	auto Application::render() -> void
	{
		auto& glContext = mWindow->getOpenGLContext();
		glContext.clear(mBackgroundColor);
		glContext.display();
	}

	auto Application::initEventCallback() -> void
	{
		mEventManager.registerHandler<EventType::Close>([this](const EventType::Close&) { mWindow->close(); });
	}
} // namespace HJUIK

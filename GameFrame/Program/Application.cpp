#include "Application.hpp"

#include "../Graphics/Color.hpp"
#include "Properties.hpp"

#include "State/MenuState.hpp"
#include "State/GameState.hpp"
#include "State/PauseState.hpp"

namespace HJUIK
{
	Application::Application() : mUpdateInterval(getUpdateInterval())
	{
		mWindow = std::make_unique<SFMLWindow>(getDefaultWindowSize(), getWindowTitle(), getWindowType());
		mWindow->limitFrameRate(getFramerate());
		mWindow->setKeyRepeatable(getKeyRepeatability());

		initEventCallback();
		initStateMachine();

		// Currently our program will be in Wjbu mode
        // NOLINTNEXTLINE(*-magic-numbers)
        mWindow->getOpenGLContext().getBaseColor() = Graphics::Color{ 0xFFBCB3FF };
	}

	auto Application::run() -> void
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

                if (mStateVector.isEmpty())
				{
                    mWindow->close();
                }
			}

			render();
		}
	}

	auto Application::update(Time deltaTime) -> void
    {
		mStateVector.update(deltaTime);
	}


	auto Application::processInput() -> void
	{
		Event event;
		while (mWindow->pollEvent(event)) {
			// I think I must let each state have their own event manager. The reason being
            // the blocking mechanism (in `FSMState.hpp`). I couldn't think of a possible way
            // if all handler is equivalent inside `mEventManager`
            mStateVector.processInput(event);
			mEventManager.processEvent(event);
		}
	}

	auto Application::render() -> void
	{
		auto& glContext = mWindow->getOpenGLContext();
		glContext.clear();

		mStateVector.render(glContext);

		glContext.display();
	}

	auto Application::initEventCallback() -> void
	{
		mEventManager.registerHandler<EventType::Close>([this](const EventType::Close&) { mWindow->close(); });
	}

    auto Application::initStateMachine() -> void
    {
        // NOLINTBEGIN(*-magic-numbers)
		mStateVector.registerState<FSM::MenuState>( "Menu",
			1003, "This is a place holder content. In pratice, the content is displayed as graphics");
        mStateVector.registerState<FSM::MenuState>( "Menu 2",
			1234, "This is not the first menu state you saw");
        mStateVector.registerState<FSM::GameState>( "Game",
			12344234, "This is the beginning of the game state (this text is `mContent`)");
        mStateVector.registerState<FSM::PauseState>( "Pause", 9234);
		mStateVector.push("Menu");
        // NOLINTEND(*-magic-numbers)
	}
} // namespace HJUIK

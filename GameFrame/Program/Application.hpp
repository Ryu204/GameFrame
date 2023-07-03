#ifndef GAMEFRAME_PROGRAM_APPLICATION_HPP
#define GAMEFRAME_PROGRAM_APPLICATION_HPP

/*
	Application is the highest layer of wrapper and will be used in main() function.
*/

#include <memory>

#include "../Graphics/Buffer.hpp"
#include "../Assets/ShaderProgramAsset.hpp"
#include "../Graphics/VertexArray.hpp"
#include "../Utilize.hpp"
#include "../Window.hpp"

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

		std::unique_ptr<IWindow> mWindow;
		EventManager mEventManager;
		const Time mUpdateInterval;

		Graphics::VertexArray mVAO;
		Graphics::Buffer mVBO;
		Graphics::Buffer mUBO;
        std::shared_ptr<Assets::ShaderProgramAsset> mProgram;

        Graphics::PossiblyBoundVertexArray mBoundVAO;
        Graphics::PossiblyBoundBuffer mBoundUBO;
	};
} // namespace HJUIK

#endif

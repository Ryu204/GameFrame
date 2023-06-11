#include "Application.hpp"

#include "../Graphics/Color.hpp"
#include "Properties.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Uniform {
	glm::mat<4, 4, float> Transform;
};

namespace HJUIK
{
	Application::Application()
		: mUpdateInterval(getUpdateInterval()), mWindow{std::make_unique<SFMLWindow>(
													getDefaultWindowSize(), getWindowTitle(), getWindowType())},
		  mProgram{[] {
			  const auto vertexShader = Graphics::VertexShader::createAndCompile(R"(
                    #version 330
                    in vec2 position;

                    out vec4 color;

                    layout (std140) uniform Uniform {
                        mat4 transform;
                    };

                    void main() {
                        gl_Position = transform * vec4(position, 0.0, 1.0);
                        color = vec4(position + 0.5, 1.0, 1.0);
                    }
                )");

			  const auto fragmentShader = Graphics::FragmentShader::createAndCompile(R"(
                    #version 330
                    in vec4 color;

                    out vec4 outColor;

                    void main() {
                        outColor = color;
                    }
                )");

			  return Graphics::Program::createVertexFragment(vertexShader, fragmentShader);
		  }()}
	{
		mWindow->limitFrameRate(getFramerate());
		mWindow->setKeyRepeatable(getKeyRepeatability());

		initEventCallback();

		// Currently our program will be in Wjbu mode
		// NOLINTNEXTLINE(*-magic-numbers)
		mWindow->getOpenGLContext().getBaseColor() = Graphics::Color{0xFFBCB3FF};

		mVAO.bind();
		const auto index = mProgram.getUniformBlockIndex("Uniform");
		if (index >= 0) {
			mUBO.bindBase(Graphics::BufferTarget::UNIFORM, 0);
		}

		struct Vertex {
			Vector2f Position;
		};
		mVBO.bind(Graphics::BufferTarget::ARRAY);
		mProgram.bind();

		const std::array<Vertex, 4> vboContent{// NOLINTNEXTLINE(*-magic-numbers)
			Vertex{{0.5F, 0.5F}}, {{-0.5F, 0.5F}}, {{0.5F, -0.5F}}, {{-0.5F, -0.5F}}};
		Graphics::BufferUsage usage{};
		// no need to configure `usage` because its default behavior already fits what we needed
		Graphics::Buffer::allocate(Graphics::BufferTarget::ARRAY, usage, vboContent);

		Graphics::VertexArray::floatAttribPointer(
			0, 2, Graphics::VertexAttribFloatType::FLOAT, /*normalize*/ false, sizeof(Vertex), 0);
		Graphics::VertexArray::enableAttrib(0);


		mUBO.bind(Graphics::BufferTarget::UNIFORM);
		mUBO.bindBase(Graphics::BufferTarget::UNIFORM, 0);
		usage.Frequency		 = Graphics::BufferAccessFrequency::DYNAMIC;
		usage.DynamicStorage = true;
		Graphics::Buffer::allocate(Graphics::BufferTarget::UNIFORM, usage, sizeof(Uniform));
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
		static Clock clock;
		auto& glContext = mWindow->getOpenGLContext();
		glContext.clear();
		const auto size = mWindow->getSize();
		glViewport(0, 0, static_cast<GLsizei>(size.x), static_cast<GLsizei>(size.y));
		Uniform uniform{};
		// NOLINTNEXTLINE(*-magic-numbers)
		uniform.Transform = glm::rotate(glm::mat4{1.0F}, clock.total().toSecond(), glm::vec3{0, 0, 1});
		std::cout << clock.total().toSecond() << std::endl;
		Graphics::Buffer::memCopy(Graphics::BufferTarget::UNIFORM, 0, std::array<Uniform, 1>{uniform});
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glContext.display();
	}

	auto Application::initEventCallback() -> void
	{
		mEventManager.registerHandler<EventType::Close>([this](const EventType::Close&) { mWindow->close(); });
	}
} // namespace HJUIK

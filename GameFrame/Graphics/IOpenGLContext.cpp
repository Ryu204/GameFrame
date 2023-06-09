#include "IOpenGLContext.hpp"

#include "glad/glad.h"
#include <iostream>

namespace HJUIK
{
	namespace Graphics
	{
		IOpenGLContext::IOpenGLContext()
		{
			loadOpenGL();
			setupDebugLogger();
		}

		auto IOpenGLContext::mOpenGLLoaded() -> bool&
		{
			static bool loaded = false;
			return loaded;
		}

		auto IOpenGLContext::loadOpenGL() -> void
		{
			if (!mOpenGLLoaded()) {
				gladLoadGL();
				mOpenGLLoaded() = true;
			}
		}

		// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
		auto IOpenGLContext::clear(Color color) -> void
		{
			auto normalized = color.getNormalizedColor();
			glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

        auto IOpenGLContext::clear() -> void
        {
            const auto normalized = mBaseColor.getNormalizedColor();
            glClearColor(normalized.r, normalized.g, normalized.b, normalized.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        auto IOpenGLContext::getBaseColor() -> Color &
        {
            return mBaseColor;
        }

		auto IOpenGLContext::setupDebugLogger() -> bool
		{
#ifndef NDEBUG
			if (GLAD_GL_VERSION_4_3 != 0) {
				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
				glDebugMessageCallback(
					// NOLINTNEXTLINE(bugprone-easily-swappable-parameters, readability-identifier-length)
					[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/,
						const GLchar* message, const void* /*userParam*/) {
						const auto* sourceStr = [source]() {
							switch (source) {
							case GL_DEBUG_SOURCE_API:
								return "API";
							case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
								return "WINDOW SYSTEM";
							case GL_DEBUG_SOURCE_SHADER_COMPILER:
								return "SHADER COMPILER";
							case GL_DEBUG_SOURCE_THIRD_PARTY:
								return "THIRD PARTY";
							case GL_DEBUG_SOURCE_APPLICATION:
								return "APPLICATION";
							default:
								return "OTHER";
							}
						}();

						const auto* typeStr = [type]() {
							switch (type) {
							case GL_DEBUG_TYPE_ERROR:
								return "ERROR";
							case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
								return "DEPRECATED_BEHAVIOR";
							case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
								return "UNDEFINED_BEHAVIOR";
							case GL_DEBUG_TYPE_PORTABILITY:
								return "PORTABILITY";
							case GL_DEBUG_TYPE_PERFORMANCE:
								return "PERFORMANCE";
							case GL_DEBUG_TYPE_MARKER:
								return "MARKER";
							default:
								return "OTHER";
							}
						}();

						const auto* severityStr = [severity]() {
							switch (severity) {
							case GL_DEBUG_SEVERITY_NOTIFICATION:
								return "NOTIFICATION";
							case GL_DEBUG_SEVERITY_LOW:
								return "LOW";
							case GL_DEBUG_SEVERITY_MEDIUM:
								return "MEDIUM";
							case GL_DEBUG_SEVERITY_HIGH:
								return "HIGH";
							default:
								return "UNKNOWN";
							}
						}();
						std::cout << sourceStr << ", " << typeStr << ", " << severityStr << ", " << id << ": "
								  << message << '\n';
					},
					nullptr);
				return true;
			}
#endif
			return false;
		}
	} // namespace Graphics
} // namespace HJUIK

#ifndef GAMEFRAME_GRAPHICS_OPENGL_CONTEXT_HPP
#define GAMEFRAME_GRAPHICS_OPENGL_CONTEXT_HPP

#include "Color.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		class IOpenGLContext
		{
		public:
			IOpenGLContext();
			virtual ~IOpenGLContext() = default;

			IOpenGLContext(const IOpenGLContext&)					 = delete;
			IOpenGLContext(IOpenGLContext&&)						 = delete;
			auto operator=(const IOpenGLContext&) -> IOpenGLContext& = delete;
			auto operator=(IOpenGLContext&&) -> IOpenGLContext&		 = delete;

			// Clear the current framebuffer with a specified color
			auto clear(Color color) -> void;
            // Clear the current framebuffer with default color
            auto clear() -> void;

            // Get the default color
            auto getBaseColor() -> Graphics::Color &;

			// Display the content after finished rendering
			virtual auto display() -> void = 0;

		private:
			static auto setupDebugLogger() -> bool;
			static auto loadOpenGL() -> void;

			// OpenGL status
			static auto mOpenGLLoaded() -> bool&;

            // Default color of the context
            Graphics::Color mBaseColor;
		};
	} // namespace Graphics
} // namespace HJUIK

#endif

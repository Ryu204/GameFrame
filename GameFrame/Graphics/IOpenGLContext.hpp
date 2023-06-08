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

			// Clear the current framebuffer
			auto clear(Color color) -> void;

			// Display the content after finished rendering
			virtual auto display() -> void = 0;

		  private:
			static auto setupDebugLogger() -> bool;
			static auto loadOpenGL() -> void;

			// OpenGL status
			static auto mOpenGLLoaded() -> bool&;
		};
	} // namespace Graphics
} // namespace HJUIK

#endif

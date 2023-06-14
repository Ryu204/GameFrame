#ifndef GAMEFRAME_GRAPHICS_VERTEX_ARRAY_HPP
#define GAMEFRAME_GRAPHICS_VERTEX_ARRAY_HPP

#include "deps/glad/glad.h"

#include "OpenGLWrapper.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		namespace detail
		{
			struct VertexArrayTrait {
				using HandleType = GLuint;
				static auto create() -> GLuint;

				static auto destroy(GLuint handle) -> void;
			};
		} // namespace detail

		class VertexArray : public OpenGLWrapper<detail::VertexArrayTrait>
		{
		public:
			// inherits base constructors and assigments
			using OpenGLWrapper::OpenGLWrapper;
			using OpenGLWrapper::operator=;

			auto bind() const -> void;
			static auto unbind() -> void;

			// get handle to currently bound VertexArray
			// this can be used to restore binding state after
			// temporarily binding a VertexArray
			static auto getCurrentBound() -> GLuint;

			// set a label for this VertexArray via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) const -> void;

			// operates on the current bound VAO
			// if one prefers a more OOP-ish way of doing this,
			// consider using DSA (OpenGL 4.5+)

			// wraps glEnableVertexAttribArray/glDisableVertexAttribArray
			static auto enableAttrib(std::size_t index) -> void;
			static auto disableAttrib(std::size_t index) -> void;

			// wraps glVertexAttrib*Pointer
			// required a buffer to be already bound with target GL_ARRAY_BUFFER
			static auto floatAttribPointer(std::size_t index, std::size_t size, GLenum type, bool normalize,
				std::size_t stride, std::size_t offset) -> void;
			static auto intAttribPointer(
				std::size_t index, std::size_t size, GLenum type, std::size_t stride, std::size_t offset) -> void;
			static auto doubleAttribPointer(
				std::size_t index, std::size_t size, GLenum type, std::size_t stride, std::size_t offset) -> void;
		};
	} // namespace Graphics

} // namespace HJUIK

#endif

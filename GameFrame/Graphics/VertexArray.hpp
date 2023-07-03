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

				static auto getCurrentBound() -> GLuint;
				static auto bind(GLuint handle) -> void;
			};
		} // namespace detail

		enum class VertexAttribIntType {
			BYTE		   = GL_BYTE,
			UNSIGNED_BYTE  = GL_UNSIGNED_BYTE,
			SHORT		   = GL_SHORT,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			INT			   = GL_INT,
			UNSIGNED_INT   = GL_UNSIGNED_INT,
		};

		enum class VertexAttribFloatType {
			HALF_FLOAT					 = GL_HALF_FLOAT,
			FLOAT						 = GL_FLOAT,
			DOUBLE						 = GL_DOUBLE,
			FIXED						 = GL_FIXED,
			INT_2_10_10_10_REV			 = GL_INT_2_10_10_10_REV,
			UNSIGNED_INT_2_10_10_10_REV	 = GL_UNSIGNED_INT_2_10_10_10_REV,
			UNSIGNED_INT_10F_11F_11F_REV = GL_UNSIGNED_INT_10F_11F_11F_REV
		};

		class PossiblyBoundVertexArray : public PossiblyBoundOpenGLWrapper<detail::VertexArrayTrait>
		{
		public:
			using PossiblyBoundOpenGLWrapper::PossiblyBoundOpenGLWrapper;

			// wraps glEnableVertexAttribArray/glDisableVertexAttribArray
			auto enableAttrib(std::size_t index) const -> void;
			auto disableAttrib(std::size_t index) const -> void;

			// wraps glVertexAttrib*Pointer
			// required a buffer to be already bound with target GL_ARRAY_BUFFER
			auto floatAttribPointer(std::size_t index, std::size_t size, VertexAttribFloatType type, bool normalize,
				std::size_t stride, std::size_t offset) const -> void;
			auto floatAttribPointer(std::size_t index, std::size_t size, VertexAttribIntType type, bool normalize,
				std::size_t stride, std::size_t offset) const -> void;
			auto intAttribPointer(std::size_t index, std::size_t size, VertexAttribIntType type, std::size_t stride,
				std::size_t offset) const -> void;
			auto doubleAttribPointer(std::size_t index, std::size_t size, std::size_t stride, std::size_t offset) const
				-> void;
		};

		class VertexArray : public OpenGLWrapper<detail::VertexArrayTrait, PossiblyBoundVertexArray>
		{
		public:
			// inherits base constructors and assigments
			using OpenGLWrapper::OpenGLWrapper;
			using OpenGLWrapper::operator=;

			// set a label for this VertexArray via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) const -> void;
		};
	} // namespace Graphics

} // namespace HJUIK

#endif

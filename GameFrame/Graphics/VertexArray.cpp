#include "VertexArray.hpp"

#include "../Utilize/CallAssert.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		auto detail::VertexArrayTrait::create() -> GLuint
		{
			return Utilize::throwIfZero(callGLGen<GLuint>(glGenVertexArrays), "unable to generate vertex arrays");
		}
		auto detail::VertexArrayTrait::destroy(GLuint handle) -> void
		{
			glDeleteVertexArrays(1, &handle);
		}
		auto detail::VertexArrayTrait::getCurrentBound() -> GLuint
		{
			return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, GL_VERTEX_ARRAY_BINDING));
		}
		auto detail::VertexArrayTrait::bind(GLuint handle) -> void
		{
			glBindVertexArray(handle);
		}

		auto VertexArray::setLabel(const char* name) const -> void
		{
			if (GLAD_GL_VERSION_4_3 != 0) {
				const auto guard = bind();
				glObjectLabel(GL_VERTEX_ARRAY, get(), -1, name);
			}
		}


		// this is by design
		// NOLINTBEGIN(*-member-functions-to-static)
		auto BoundVertexArray::enableAttrib(std::size_t index) const -> void
		{
			glEnableVertexAttribArray(static_cast<GLuint>(index));
		}

		auto BoundVertexArray::disableAttrib(std::size_t index) const -> void
		{
			glDisableVertexAttribArray(static_cast<GLuint>(index));
		}

		// NOLINTBEGIN(*-reinterpret-cast, *-no-int-to-ptr)
		auto BoundVertexArray::intAttribPointer(std::size_t index, std::size_t size, VertexAttribIntType type,
			std::size_t stride, std::size_t offset) const -> void
		{
			glVertexAttribIPointer(static_cast<GLuint>(index), static_cast<GLint>(size), static_cast<GLenum>(type),
				static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
		}

		auto BoundVertexArray::floatAttribPointer(std::size_t index, std::size_t size, VertexAttribIntType type,
			bool normalize, std::size_t stride, std::size_t offset) const -> void
		{
			glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(size), static_cast<GLenum>(type),
				static_cast<GLboolean>(normalize), static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
		}

		auto BoundVertexArray::floatAttribPointer(std::size_t index, std::size_t size, VertexAttribFloatType type,
			bool normalize, std::size_t stride, std::size_t offset) const -> void
		{
			glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(size), static_cast<GLenum>(type),
				static_cast<GLboolean>(normalize), static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
		}

		auto BoundVertexArray::doubleAttribPointer(
			std::size_t index, std::size_t size, std::size_t stride, std::size_t offset) const -> void
		{
			// `type` is always GL_DOUBLE according to specs
			glVertexAttribLPointer(static_cast<GLuint>(index), static_cast<GLint>(size), GL_DOUBLE,
				static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
		}
		// NOLINTEND(*-reinterpret-cast, *-no-int-to-ptr)
		// NOLINTEND(*-member-functions-to-static)
	} // namespace Graphics

} // namespace HJUIK

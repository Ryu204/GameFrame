#include "VertexArray.hpp"

namespace HJUIK {
  namespace Graphics {
    auto detail::VertexArrayTrait::create() -> GLuint {
      return checkNonZero(callGLGen<GLuint>(glGenVertexArrays), "unable to generate vertex arrays");
    }
    auto detail::VertexArrayTrait::destroy(GLuint handle) -> void {
      glDeleteVertexArrays(1, &handle);
    }

    auto VertexArray::bind() const -> void {
      glBindVertexArray(get());
    }
    auto VertexArray::unbind() -> void {
      glBindVertexArray(0);
    }
    auto VertexArray::getCurrentBound() -> GLuint {
      return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, GL_VERTEX_ARRAY_BINDING));
    }
    auto VertexArray::setLabel(const char* name) const -> void {
      if (GLAD_GL_VERSION_4_3 != 0) {
        auto currentBound = getCurrentBound();
        bind();
        glObjectLabel(GL_VERTEX_ARRAY, get(), -1, name);
        glBindVertexArray(currentBound);
      }
    }

    auto VertexArray::enableAttrib(std::size_t index) -> void {
      glEnableVertexAttribArray(static_cast<GLuint>(index));
    }

    auto VertexArray::disableAttrib(std::size_t index) -> void {
      glDisableVertexAttribArray(static_cast<GLuint>(index));
    }

    // NOLINTBEGIN(*-reinterpret-cast, *-no-int-to-ptr)
    auto VertexArray::intAttribPointer(std::size_t index, std::size_t size, GLenum type, std::size_t stride, std::size_t offset) -> void {
      glVertexAttribIPointer(static_cast<GLuint>(index), static_cast<GLint>(size), type, static_cast<GLsizei>(stride),
          reinterpret_cast<const void*>(offset));
    }

    static auto floatAttribPointer(std::size_t index, std::size_t size, GLenum type, bool normalize, std::size_t stride, std::size_t offset)
        -> void {
      glVertexAttribPointer(static_cast<GLuint>(index), static_cast<GLint>(size), type,
          static_cast<GLboolean>(normalize), static_cast<GLsizei>(stride), reinterpret_cast<const void*>(offset));
    }

    static auto doubleAttribPointer(std::size_t index, std::size_t size, GLenum type, std::size_t stride, std::size_t offset) -> void {
      glVertexAttribLPointer(static_cast<GLuint>(index), static_cast<GLint>(size), type, static_cast<GLsizei>(stride),
          reinterpret_cast<const void*>(offset));
    }
    // NOLINTEND(*-reinterpret-cast, *-no-int-to-ptr)
  } // namespace Graphics

} // namespace HJUIK

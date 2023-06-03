#ifndef GAMEFRAME_GRAPHICS_VERTEX_ARRAY_HPP
#define GAMEFRAME_GRAPHICS_VERTEX_ARRAY_HPP

#include "OpenGLWrapper.hpp"
#include "glad/glad.h"

namespace HJUIK
{
  namespace Graphics
  {
    namespace detail
    {
      struct VertexArrayTrait
      {
        using HandleType = GLuint;
        static auto create() -> GLuint
        {
          return checkNonZero(callGLGen<GLuint>(glGenVertexArrays),
                              "unable to generate vertex arrays");
        }

        static auto destroy(GLuint handle) -> void { glDeleteVertexArrays(1, &handle); }
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
    };
  } // namespace Graphics

} // namespace HJUIK

#endif

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
        static GLuint create()
        {
          return checkNonZero(callGLGen<GLuint>(glGenVertexArrays),
                              "unable to generate vertex arrays");
        }

        static void destroy(GLuint handle) { glDeleteVertexArrays(1, &handle); }
      };
    } // namespace detail

    class VertexArray : public OpenGLWrapper<detail::VertexArrayTrait>
    {
    public:
      // inherits base constructors and assigments
      using OpenGLWrapper::OpenGLWrapper;
      using OpenGLWrapper::operator=;

      auto bind() -> void const;
      static auto unbind() -> void const;
    };
  } // namespace Graphics

} // namespace HJUIK

#endif

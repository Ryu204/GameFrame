#ifndef GAMEFRAME_GRAPHICS_PROGRAM_HPP
#define GAMEFRAME_GRAPHICS_PROGRAM_HPP

#include "glad/glad.h"
#include <vector>

#include "OpenGLWrapper.hpp"
#include "Shader.hpp"

namespace HJUIK {
  namespace Graphics {
    namespace detail {
      struct ProgramTrait {
        using HandleType = GLuint;
        static auto create() -> GLuint;
        static auto destroy(GLuint handle) -> void;
      };
    } // namespace detail

    class Program : public OpenGLWrapper<detail::ProgramTrait> {
    public:
      using OpenGLWrapper::OpenGLWrapper;
      using OpenGLWrapper::operator=;

      // the most used kind of shader program, containing a vertex shader
      // and a fragment shader
      static auto createVertexFragment(const VertexShader& vertexShader, const FragmentShader& fragmentShader)
          -> Program;

      // set a label for this Program via `glObjectLabel`.
      // this label may show up in debug callback or an external OpenGL
      // debugger (e.g. RenderDoc)
      // (only have effect in OpenGL 4.3+)
      auto setLabel(const char* name) const -> void;

      // wrappers/utility functions for glAttachShader/glDetachShader
      auto attachShader(GLuint shaderHandle) const -> void;
      auto detachShader(GLuint shaderHandle) const -> void;
      auto getAllAttachedShaderHandles() const -> std::vector<GLuint>;
      auto detachAllShaders() const -> void;

      template <ShaderType Type>
      auto attachShader(const Shader<Type>& shader) const -> void {
        attachShader(shader.get());
      }
      template <ShaderType Type>
      auto detachShader(const Shader<Type>& shader) const -> void {
        detachShader(shader.get());
      }

      // wrapper for glLinkProgram
      // also return the link status
      auto link() const -> bool;

      // wrapper for glGetProgramiv
      auto getInt(GLenum pname) const -> GLint;

      // wrapper for glGetProgramInfoLog
      auto getInfoLog() const -> std::string;

      auto use() const -> void;
      static auto unuse() -> void;
      static auto getCurrentUsed() -> GLuint;

      // -1 if attribute/uniform is not present
      auto getAttribLocation(const char* name) const -> std::ptrdiff_t;
      auto getUniformLocation(const char* name) const -> std::ptrdiff_t;

      // wrappers for glGetUniformBlockIndex/glUniformBlockBinding
      auto getUniformBlockIndex(const char* name) const -> std::ptrdiff_t;
      auto setUniformBlockBinding(std::size_t index, std::size_t binding) -> void;
      auto setUniformBlockBinding(const char* name, std::size_t binding) -> void;
    };
  } // namespace Graphics

} // namespace HJUIK


#endif

#ifndef GAMEFRAME_GRAPHICS_SHADER_HPP
#define GAMEFRAME_GRAPHICS_SHADER_HPP

#include "glad/glad.h"

#include "OpenGLWrapper.hpp"

namespace HJUIK {
  namespace Graphics {
    enum class ShaderType {
      VERTEX          = GL_VERTEX_SHADER,
      FRAGMENT        = GL_FRAGMENT_SHADER,
      GEOMETRY        = GL_GEOMETRY_SHADER,
      COMPUTE         = GL_COMPUTE_SHADER,
      TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
      TESS_CONTROL    = GL_TESS_CONTROL_SHADER,
    };

    namespace detail {
      template <ShaderType Type>
      struct ShaderTrait {
        using HandleType = GLuint;
        static auto create() -> GLuint;
        static auto destroy(GLuint handle);
      };
    } // namespace detail

    template <ShaderType Type>
    class Shader : public OpenGLWrapper<detail::ShaderTrait<Type>> {
    public:
      using OpenGLWrapper<detail::ShaderTrait<Type>>::OpenGLWrapper;
      using OpenGLWrapper<detail::ShaderTrait<Type>>::operator=;

      static auto createAndCompile(const char* source) -> Shader;

      // set a label for this Shader via `glObjectLabel`.
      // this label may show up in debug callback or an external OpenGL
      // debugger (e.g. RenderDoc)
      // (only have effect in OpenGL 4.3+)
      auto setLabel(const char* name) const -> void;

      auto setSource(const char* source) const -> void;

      // wrapper for glCompileShader
      // also return the compile status
      auto compile() const -> bool;

      // wrapper for glGetShaderiv
      auto getInt(GLenum pname) const -> GLint;

      // wrapper for glGetShaderInfoLog
      auto getInfoLog() const -> std::string;
    };

    using VertexShader         = Shader<ShaderType::VERTEX>;
    using FragmentShader       = Shader<ShaderType::FRAGMENT>;
    using GeometryShader       = Shader<ShaderType::GEOMETRY>;
    using ComputeShader        = Shader<ShaderType::COMPUTE>;
    using TessControlShader    = Shader<ShaderType::TESS_CONTROL>;
    using TessEvaluationShader = Shader<ShaderType::TESS_EVALUATION>;

  } // namespace Graphics
} // namespace HJUIK

#include "Shader.inl"

#endif

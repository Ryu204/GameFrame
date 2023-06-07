#include "Shader.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    template <ShaderType Type>
    auto detail::ShaderTrait<Type>::create() -> GLuint
    {
      return glCreateShader(static_cast<GLenum>(Type));
    }

    template <ShaderType Type>
    auto detail::ShaderTrait<Type>::destroy(GLuint handle)
    {
      glDeleteShader(handle);
    }

    template <ShaderType Type>
    auto Shader<Type>::createAndCompile(const char* source) -> Shader
    {
      Shader shader;
      shader.setSource(source);
      if (!shader.compile()) {
        const auto infoLog = shader.getInfoLog();
        throw std::runtime_error("unable to compile shader: " + infoLog);
      }
      return shader;
    }

    template <ShaderType Type>
    auto Shader<Type>::setLabel(const char* name) const -> void
    {
      // no need for binding the shader because it was already
      // created when one calls glCreateShader
      glObjectLabel(GL_SHADER, this->get(), -1, name);
    }

    template <ShaderType Type>
    inline auto Shader<Type>::setSource(const char* source) const -> void
    {
      glShaderSource(this->get(), 1, &source, nullptr);
    }

    template <ShaderType Type>
    inline auto Shader<Type>::compile() const -> bool
    {
      glCompileShader(this->get());
      return getInt(GL_COMPILE_STATUS) != GL_FALSE;
    }

    template <ShaderType Type>
    inline auto Shader<Type>::getInt(GLenum pname) const -> GLint
    {
      return callGLGet<GLint>(glGetShaderiv, this->get(), pname);
    }

    template <ShaderType Type>
    inline auto Shader<Type>::getInfoLog() const -> std::string
    {
      auto length = getInt(GL_INFO_LOG_LENGTH);
      // `length` is either 0 (no info log) or >= 1 (the length of
      // the info log INCLUDING the null termination character)
      if (length >= 1) {
        length--;
      }

      std::string infoLog;
      infoLog.resize(length);
      glGetShaderInfoLog(this->get(), length + 1, nullptr, infoLog.data());

      return infoLog;
    }

  } // namespace Graphics
} // namespace HJUIK

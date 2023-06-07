#include "Program.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    auto HJUIK::Graphics::detail::ProgramTrait::create() -> GLuint
    {
      return glCreateProgram();
    }

    auto detail::ProgramTrait::destroy(GLuint handle) -> void
    {
      glDeleteProgram(handle);
    }

    auto Program::createVertexFragment(const VertexShader& vertexShader, const FragmentShader& fragmentShader)
        -> Program
    {
      Program program{};
      program.attachShader(vertexShader);
      program.attachShader(fragmentShader);
      if (!program.link()) {
        throw std::runtime_error("unable to link program: " + program.getInfoLog());
      }
      program.detachAllShaders();
      return program;
    }

    auto Program::setLabel(const char* name) const -> void
    {
      // no need for binding the program because it was already
      // created when one calls glCreateProgram
      glObjectLabel(GL_PROGRAM, get(), -1, name);
    }

    auto Program::attachShader(GLuint shaderHandle) const -> void
    {
      glAttachShader(get(), shaderHandle);
    }

    auto Program::detachShader(GLuint shaderHandle) const -> void
    {
      glAttachShader(get(), shaderHandle);
    }

    auto Program::getAllAttachedShaderHandles() const -> std::vector<GLuint>
    {
      const auto numShaders = static_cast<std::size_t>(getInt(GL_ATTACHED_SHADERS));
      std::vector<GLuint> shaders;
      shaders.resize(numShaders);
      glGetAttachedShaders(get(), static_cast<GLsizei>(numShaders), nullptr, shaders.data());
      return shaders;
    }

    auto Program::detachAllShaders() const -> void
    {
      for (const auto handler : getAllAttachedShaderHandles()) {
        detachShader(handler);
      }
    }

    auto Program::link() const -> bool
    {
      glLinkProgram(get());
      return getInt(GL_LINK_STATUS) != GL_FALSE;
    }

    auto Program::getInt(GLenum pname) const -> GLint
    {
      return callGLGet<GLint>(glGetProgramiv, get(), pname);
    }

    auto Program::getInfoLog() const -> std::string
    {
      auto length = static_cast<std::size_t>(getInt(GL_INFO_LOG_LENGTH));
      if (length >= 1) {
        length--;
      }

      std::string infoLog;
      infoLog.resize(length);

      glGetProgramInfoLog(get(), static_cast<GLsizei>(length + 1), nullptr, infoLog.data());
      return infoLog;
    }

    auto Program::use() const -> void
    {
      glUseProgram(get());
    }

    auto Program::unuse() -> void
    {
      glUseProgram(0);
    }

    auto Program::getCurrentUsed() -> GLuint
    {
      return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, GL_CURRENT_PROGRAM));
    }

    auto Program::getAttribLocation(const char* name) const -> std::ptrdiff_t
    {
      return static_cast<std::ptrdiff_t>(glGetAttribLocation(get(), name));
    }
    auto Program::getUniformLocation(const char* name) const -> std::ptrdiff_t
    {
      return static_cast<std::ptrdiff_t>(glGetUniformLocation(get(), name));
    }
    auto Program::getUniformBlockIndex(const char* name) const -> std::ptrdiff_t
    {
      return static_cast<std::ptrdiff_t>(glGetUniformBlockIndex(get(), name));
    }
    auto Program::setUniformBlockBinding(std::size_t index, std::size_t binding) -> void
    {
      return glUniformBlockBinding(get(), static_cast<GLuint>(index), static_cast<GLuint>(binding));
    }
    auto Program::setUniformBlockBinding(const char* name, std::size_t binding) -> void
    {
      const auto index = getUniformBlockIndex(name);
      if (index >= 0) {
        setUniformBlockBinding(index, binding);
      }
    }
  } // namespace Graphics
} // namespace HJUIK

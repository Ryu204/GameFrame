#include "Framebuffer.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    auto detail::FramebufferTrait::create() -> GLuint
    {
      return callGLGen<GLuint>(glGenFramebuffers);
    }

    auto detail::FramebufferTrait::destroy(GLuint handle) -> void
    {
      glDeleteFramebuffers(1, &handle);
    }

    auto detail::RenderbufferTrait::create() -> GLuint
    {
      return callGLGen<GLuint>(glGenRenderbuffers);
    }

    auto detail::RenderbufferTrait::destroy(GLuint handle) -> void
    {
      glDeleteRenderbuffers(1, &handle);
    }

    auto Renderbuffer::bind() const -> void
    {
      glBindRenderbuffer(GL_RENDERBUFFER, get());
    }

    auto Renderbuffer::unbind() -> void
    {
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    auto Renderbuffer::getCurrentBound() -> GLuint
    {
      return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, GL_RENDERBUFFER_BINDING));
    }

    auto Renderbuffer::allocStorage(
        TextureInternalFormat internalFormat, size_t width, size_t height, std::ptrdiff_t samples) -> void
    {
      if (samples >= 0) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, static_cast<GLsizei>(samples),
            static_cast<GLenum>(internalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
      } else {
        glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(internalFormat), static_cast<GLsizei>(width),
            static_cast<GLsizei>(height));
      }
    }

    auto Framebuffer::bind(FramebufferTarget target) const -> void
    {
      glBindFramebuffer(static_cast<GLenum>(target), get());
    }

    auto Framebuffer::unbind(FramebufferTarget target) -> void
    {
      glBindFramebuffer(static_cast<GLenum>(target), 0);
    }

    auto Framebuffer::getCurrentBound(FramebufferTarget target) -> GLuint
    {
      const auto pName = target == FramebufferTarget::READ ? GL_READ_FRAMEBUFFER_BINDING : GL_DRAW_FRAMEBUFFER_BINDING;
      return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, pName));
    }

    auto Framebuffer::setLabel(const char* name) const -> void
    {
      static constexpr auto tempTarget = FramebufferTarget::READ;
      if (GLAD_GL_VERSION_4_3 != 0) {
        auto currentBound = getCurrentBound(tempTarget);
        bind(tempTarget);
        glObjectLabel(GL_VERTEX_ARRAY, get(), -1, name);
        glBindFramebuffer(static_cast<GLenum>(tempTarget), currentBound);
      }
    }

    auto Framebuffer::setRenderbufferAttachment(
        FramebufferTarget target, FramebufferAttachment attachment, const Renderbuffer& renderbuffer) -> void
    {
      glFramebufferRenderbuffer(
          static_cast<GLenum>(target), framebufferAttachmentAsGLEnum(attachment), GL_RENDERBUFFER, renderbuffer.get());
    }

  } // namespace Graphics
} // namespace HJUIK

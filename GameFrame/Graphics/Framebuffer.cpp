#include "Framebuffer.hpp"

#include <optional>

namespace HJUIK
{
	namespace Graphics
	{
		auto detail::FramebufferTrait::create() -> GLuint
		{
			return Utilize::throwIfZero(
				supportsDSA() ? callGLGen<GLuint>(glCreateFramebuffers) : callGLGen<GLuint>(glGenFramebuffers),
				"unable to create framebuffer");
		}

		auto detail::FramebufferTrait::destroy(GLuint handle) -> void
		{
			glDeleteFramebuffers(1, &handle);
		}

		auto detail::FramebufferTrait::getCurrentBound(FramebufferTarget target) -> GLuint
		{
			const auto pName =
				target == FramebufferTarget::READ ? GL_READ_FRAMEBUFFER_BINDING : GL_DRAW_FRAMEBUFFER_BINDING;
			return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, pName));
		}

		auto detail::FramebufferTrait::bind(GLuint handle, FramebufferTarget target) -> void
		{
			glBindFramebuffer(static_cast<GLenum>(target), handle);
		}

		auto detail::RenderbufferTrait::create() -> GLuint
		{
			return Utilize::throwIfZero(
				supportsDSA() ? callGLGen<GLuint>(glCreateRenderbuffers) : callGLGen<GLuint>(glGenRenderbuffers),
				"unable to create renderbuffer");
		}

		auto detail::RenderbufferTrait::destroy(GLuint handle) -> void
		{
			glDeleteRenderbuffers(1, &handle);
		}

		auto detail::RenderbufferTrait::getCurrentBound() -> GLuint
		{
			return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, GL_RENDERBUFFER_BINDING));
		}

		auto detail::RenderbufferTrait::bind(GLuint handle) -> void
		{
			glBindRenderbuffer(GL_RENDERBUFFER, handle);
		}

		auto Renderbuffer::setLabel(const char* name) const -> void
		{
			if (GLAD_GL_VERSION_4_3 != 0) {
                // only binds if this is not created with glCreate*
                // i.e. DSA is not supported
				std::optional<BoundRenderbuffer> guard;
				if (supportsDSA()) {
					guard = bind();
					guard.value().forceBind();
				}
				glObjectLabel(GL_RENDERBUFFER, get(), -1, name);
			}
		}

		// NOLINTNEXTLINE(*-member-functions-to-static)
		auto BoundRenderbuffer::allocStorage(TextureInternalFormat internalFormat, std::size_t width,
			std::size_t height, std::ptrdiff_t samples) const -> void
		{
			if (samples >= 0) {
				if (supportsDSA()) {
					glNamedRenderbufferStorageMultisample(getHandle(), static_cast<GLsizei>(samples),
						static_cast<GLenum>(internalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
				}

				forceBind();
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, static_cast<GLsizei>(samples),
					static_cast<GLenum>(internalFormat), static_cast<GLsizei>(width), static_cast<GLsizei>(height));
			} else {
				if (supportsDSA()) {
					glNamedRenderbufferStorage(getHandle(), static_cast<GLenum>(internalFormat),
						static_cast<GLsizei>(width), static_cast<GLsizei>(height));
				}

				forceBind();
				glRenderbufferStorage(GL_RENDERBUFFER, static_cast<GLenum>(internalFormat), static_cast<GLsizei>(width),
					static_cast<GLsizei>(height));
			}
		}

		auto Framebuffer::setLabel(const char* name) const -> void
		{
			static constexpr auto tempTarget = FramebufferTarget::READ;
			if (GLAD_GL_VERSION_4_3 != 0) {
                // only binds if this is not created with glCreate*
                // i.e. DSA is not supported
				std::optional<PossiblyBoundFramebuffer> guard;
				if (supportsDSA()) {
					guard = bind(tempTarget);
					guard.value().forceBind();
				}
				glObjectLabel(GL_VERTEX_ARRAY, get(), -1, name);
			}
		}

		auto PossiblyBoundFramebuffer::setRenderbufferAttachment(
			FramebufferAttachment attachment, const Renderbuffer& renderbuffer) const -> void
		{
			if (supportsDSA()) {
				glNamedFramebufferRenderbuffer(
					getHandle(), framebufferAttachmentAsGLEnum(attachment), GL_RENDERBUFFER, renderbuffer.get());
			}

			forceBind();
			glFramebufferRenderbuffer(
				getTargetEnum(), framebufferAttachmentAsGLEnum(attachment), GL_RENDERBUFFER, renderbuffer.get());
		}

		auto PossiblyBoundFramebuffer::getTarget() const -> FramebufferTarget
		{
			return std::get<0>(getArgs());
		}

		auto PossiblyBoundFramebuffer::getTargetEnum() const -> GLenum
		{
			return static_cast<GLenum>(getTarget());
		}

	} // namespace Graphics
} // namespace HJUIK

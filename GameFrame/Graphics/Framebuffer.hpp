#ifndef GAMEFRAME_FRAMEBUFFER_HPP
#define GAMEFRAME_FRAMEBUFFER_HPP

#include "deps/glad/glad.h"
#include <variant>

#include "OpenGLWrapper.hpp"
#include "Texture.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		enum class FramebufferTarget {
			READ = GL_READ_FRAMEBUFFER,
			DRAW = GL_DRAW_FRAMEBUFFER,
		};

		namespace detail
		{
			struct FramebufferTrait {
				using HandleType = GLuint;
				static auto create() -> GLuint;
				static auto destroy(GLuint handle) -> void;

				static auto getCurrentBound(FramebufferTarget target) -> GLuint;
				static auto bind(GLuint handle, FramebufferTarget target) -> void;
			};

			struct RenderbufferTrait {
				using HandleType = GLuint;
				static auto create() -> GLuint;
				static auto destroy(GLuint handle) -> void;

				static auto getCurrentBound() -> GLuint;
				static auto bind(GLuint handle) -> void;
			};
		} // namespace detail

		struct FramebufferColorAttachment {
			std::size_t Index;

			explicit operator GLenum() const
			{
				return static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + Index);
			}
		};

		struct FramebufferDepthAttachment {
			explicit operator GLenum() const
			{
				return GL_DEPTH_ATTACHMENT;
			}
		};

		struct FramebufferStencilAttachment {
			explicit operator GLenum() const
			{
				return GL_STENCIL_ATTACHMENT;
			}
		};

		struct FramebufferDepthStencilAttachment {
			explicit operator GLenum() const
			{
				return GL_DEPTH_STENCIL_ATTACHMENT;
			}
		};

		using FramebufferAttachment = std::variant<FramebufferColorAttachment, FramebufferDepthAttachment,
			FramebufferStencilAttachment, FramebufferDepthStencilAttachment>;

		inline auto framebufferAttachmentAsGLEnum(const FramebufferAttachment& attachment) -> GLenum
		{
			return std::visit([](const auto& attachment) { return static_cast<GLenum>(attachment); }, attachment);
		}

		class BoundRenderbuffer : public PossiblyBoundOpenGLWrapper<detail::RenderbufferTrait>
		{
		public:
			using PossiblyBoundOpenGLWrapper::PossiblyBoundOpenGLWrapper;
			using PossiblyBoundOpenGLWrapper::operator=;

			auto allocStorage(TextureInternalFormat internalFormat, std::size_t width, std::size_t height,
				std::ptrdiff_t samples = -1) const -> void;
		};

		class Renderbuffer : public OpenGLWrapper<detail::RenderbufferTrait, BoundRenderbuffer>
		{
		public:
			// inherits base constructors and assigments
			using OpenGLWrapper::OpenGLWrapper;
			using OpenGLWrapper::operator=;

			// set a label for this Renderbuffer via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) const -> void;
		};

		class PossiblyBoundFramebuffer : public PossiblyBoundOpenGLWrapper<detail::FramebufferTrait, FramebufferTarget>
		{
		public:
			using PossiblyBoundOpenGLWrapper::PossiblyBoundOpenGLWrapper;
			using PossiblyBoundOpenGLWrapper::operator=;

			// TODO: add support for more niche uses of this
			template <TextureType Type>
			auto setTextureAttachment(
				FramebufferAttachment attachment, const Texture<Type>& texture, std::size_t mipLevel = 0) const -> void
			{
				if (supportsDSA()) {
					glNamedFramebufferTexture(getHandle(), framebufferAttachmentAsGLEnum(attachment), texture.get(),
						static_cast<GLint>(mipLevel));
				}

				forceBind();
				glFramebufferTexture(getTargetEnum(), framebufferAttachmentAsGLEnum(attachment), texture.get(),
					static_cast<GLint>(mipLevel));
			}

			auto setRenderbufferAttachment(FramebufferAttachment attachment, const Renderbuffer& renderbuffer) const
				-> void;

			auto getTarget() const -> FramebufferTarget;

		private:
			auto getTargetEnum() const -> GLenum;
		};

		class Framebuffer : public OpenGLWrapper<detail::FramebufferTrait, PossiblyBoundFramebuffer>
		{
		public:
			// inherits base constructors and assigments
			using OpenGLWrapper::OpenGLWrapper;
			using OpenGLWrapper::operator=;

			// set a label for this Framebuffer via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) const -> void;
		};
	} // namespace Graphics

} // namespace HJUIK


#endif

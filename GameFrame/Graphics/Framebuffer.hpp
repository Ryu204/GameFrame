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
		namespace detail
		{
			struct FramebufferTrait {
				using HandleType = GLuint;
				static auto create() -> GLuint;
				static auto destroy(GLuint handle) -> void;
			};

			struct RenderbufferTrait {
				using HandleType = GLuint;
				static auto create() -> GLuint;
				static auto destroy(GLuint handle) -> void;
			};
		} // namespace detail

		enum class FramebufferTarget {
			READ = GL_READ_FRAMEBUFFER,
			DRAW = GL_DRAW_FRAMEBUFFER,
		};

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

		class Renderbuffer : public OpenGLWrapper<detail::RenderbufferTrait>
		{
		public:
			// inherits base constructors and assigments
			using OpenGLWrapper::OpenGLWrapper;
			using OpenGLWrapper::operator=;

			auto bind() const -> void;
			static auto unbind() -> void;

			// get handle to currently bound Renderbuffer
			// this can be used to restore binding state after
			// temporarily binding a Renderbuffer
			static auto getCurrentBound() -> GLuint;

			// set a label for this Renderbuffer via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) const -> void;

			static auto allocStorage(TextureInternalFormat internalFormat, std::size_t width, std::size_t height,
				std::ptrdiff_t samples = -1) -> void;
		};

		class Framebuffer : public OpenGLWrapper<detail::FramebufferTrait>
		{
		public:
			// inherits base constructors and assigments
			using OpenGLWrapper::OpenGLWrapper;
			using OpenGLWrapper::operator=;

			auto bind(FramebufferTarget target) const -> void;
			static auto unbind(FramebufferTarget target) -> void;

			// get handle to currently bound Framebuffer
			// this can be used to restore binding state after
			// temporarily binding a Framebuffer
			static auto getCurrentBound(FramebufferTarget target) -> GLuint;

			// set a label for this Framebuffer via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) const -> void;

			// TODO: add support for more niche uses of this
			template <TextureType Type>
			static auto setTextureAttachment(FramebufferTarget target, FramebufferAttachment attachment,
				const Texture<Type>& texture, std::size_t mipLevel = 0) -> void
			{
				glFramebufferTexture(static_cast<GLenum>(target), framebufferAttachmentAsGLEnum(attachment),
					texture.get(), static_cast<GLint>(mipLevel));
			}

			static auto setRenderbufferAttachment(
				FramebufferTarget target, FramebufferAttachment attachment, const Renderbuffer& renderbuffer) -> void;
		};
	} // namespace Graphics

} // namespace HJUIK


#endif

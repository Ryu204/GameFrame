#include <cstdint>
#include <optional>
#include <stdexcept>

#include "../Utilize/CallAssert.hpp"
#include "Texture.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		template <TextureType Type>
		auto detail::TextureTrait<Type>::create() -> GLuint
		{
			return Utilize::throwIfZero(
				supportsDSA() ? callGLGen<GLuint>(glCreateTextures, Type) : callGLGen<GLuint>(glGenTextures),
				"unable to create texture");
		}

		template <TextureType Type>
		auto detail::TextureTrait<Type>::destroy(GLuint handle) -> void
		{
			glDeleteTextures(1, &handle);
		}

		template <TextureType Type>
		inline auto detail::TextureTrait<Type>::getCurrentBound() -> GLuint
		{
			return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, detail::getTextureBindingType(Type)));
		}

		template <TextureType Type>
		inline auto detail::TextureTrait<Type>::bind(GLuint handle) -> void
		{
			glBindTexture(static_cast<GLenum>(Type), handle);
		}

		inline constexpr auto detail::getTextureBindingType(TextureType type) -> GLenum
		{
			switch (type) {
			case TextureType::E1D:
				return GL_TEXTURE_BINDING_1D;
			case TextureType::E2D:
				return GL_TEXTURE_BINDING_2D;
			case TextureType::E3D:
				return GL_TEXTURE_BINDING_3D;
			case TextureType::RECTANGLE:
				return GL_TEXTURE_BINDING_RECTANGLE;
			case TextureType::BUFFER:
				return GL_TEXTURE_BINDING_BUFFER;
			case TextureType::CUBE_MAP:
				return GL_TEXTURE_BINDING_CUBE_MAP;
			case TextureType::E1D_ARRAY:
				return GL_TEXTURE_BINDING_1D_ARRAY;
			case TextureType::E2D_ARRAY:
				return GL_TEXTURE_BINDING_2D_ARRAY;
			case TextureType::CUBE_MAP_ARRAY:
				return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
			case TextureType::E2D_MULTISAMPLE:
				return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
			case TextureType::E2D_MULTISAMPLE_ARRAY:
				return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
			default:
				throw std::runtime_error("invalid texture type");
			}
		}

		template <TextureType Type>
		inline auto PossiblyBoundTexture<Type>::getDimensions(std::size_t mipLevel) const -> VectorType
		{
			VectorType dimensions;
			if constexpr (NUM_DIMENSIONS >= 1) {
				dimensions.x = static_cast<std::size_t>(callGLGet<GLint>(glGetTexLevelParameteriv,
					static_cast<GLenum>(Type), static_cast<GLint>(mipLevel), GL_TEXTURE_WIDTH));
			}
			if constexpr (NUM_DIMENSIONS >= 2) {
				dimensions.y = static_cast<std::size_t>(callGLGet<GLint>(glGetTexLevelParameteriv,
					static_cast<GLenum>(Type), static_cast<GLint>(mipLevel), GL_TEXTURE_HEIGHT));
			}
			if constexpr (NUM_DIMENSIONS >= 3) {
				dimensions.z = static_cast<std::size_t>(callGLGet<GLint>(glGetTexLevelParameteriv,
					static_cast<GLenum>(Type), static_cast<GLint>(mipLevel), GL_TEXTURE_DEPTH));
			}
			return dimensions;
		}

		template <TextureType Type>
		inline auto Texture<Type>::setLabel(const char* name) -> void
		{
			if (GLAD_GL_VERSION_4_3 != 0) {
				// only binds if this is not created with glCreate*
				// i.e. DSA is not supported
				std::optional<PossiblyBoundTexture<Type>> guard;
				if (supportsDSA()) {
					guard = this->bind();
					guard.value().forceBind();
				}
				glObjectLabel(GL_TEXTURE, this->get(), -1, name);
			}
		}

		template <TextureType Type>
		inline auto Texture<Type>::bindActive(std::size_t slot) const -> PossiblyBoundTexture<Type>
		{
			glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + slot));
			return this->bind();
		}

		template <TextureType Type>
		inline auto PossiblyBoundTexture<Type>::allocate(
			const TextureAllocationInfo& allocInfo, const VectorType& dimensions) const -> void
		{
			if (supportsDSA() && allocInfo.Immutable) {
				// else branches are unsupported by OpenGL (in order to discourage non-immutable textures idk)
				// one must use `glTexImage*` (not DSA)
				// the version checks are kinda unimportant (`supportsDSA` needs GL4.5), but maybe we will
				// switch to the extension condition (`GL_ARB_direct_state_access` so I'm keeping that here)
				if constexpr (Type == TextureType::E2D_MULTISAMPLE) {
					if (GLAD_GL_VERSION_4_3 != 0) {
						glTextureStorage2DMultisample(this->getHandle(), static_cast<GLsizei>(allocInfo.Samples),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
							static_cast<GLsizei>(dimensions.y), static_cast<GLboolean>(allocInfo.FixedSampleLocations));
						return;
					}
				} else if constexpr (Type == TextureType::E2D_MULTISAMPLE_ARRAY) {
					if (GLAD_GL_VERSION_4_3 != 0) {
						glTextureStorage3DMultisample(this->getHandle(), static_cast<GLsizei>(allocInfo.Samples),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
							static_cast<GLsizei>(dimensions.y), static_cast<GLsizei>(dimensions.z),
							static_cast<GLboolean>(allocInfo.FixedSampleLocations));
					}
				} else if constexpr (NUM_DIMENSIONS == 1) {
					if (GLAD_GL_VERSION_4_2 != 0) {
						glTextureStorage1D(this->getHandle(), static_cast<GLsizei>(allocInfo.MipmapLevels),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x));
						return;
					}
				} else if constexpr (NUM_DIMENSIONS == 2) {
					if (GLAD_GL_VERSION_4_2 != 0) {
						glTextureStorage2D(this->getHandle(), static_cast<GLsizei>(allocInfo.MipmapLevels),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
							static_cast<GLsizei>(dimensions.y));
						return;
					}
				} else if constexpr (NUM_DIMENSIONS == 3) {
					if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
						glTextureStorage3D(this->getHandle(), static_cast<GLsizei>(allocInfo.MipmapLevels),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
							static_cast<GLsizei>(dimensions.y), static_cast<GLsizei>(dimensions.z));
						return;
					}
				}
			}

			this->forceBind();
			if constexpr (Type == TextureType::E2D_MULTISAMPLE) {
				if (GLAD_GL_VERSION_4_3 != 0 && allocInfo.Immutable) {
					glTexStorage2DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
						static_cast<GLsizei>(dimensions.y), static_cast<GLboolean>(allocInfo.FixedSampleLocations));
				} else {
					glTexImage2DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
						static_cast<GLsizei>(dimensions.y), static_cast<GLboolean>(allocInfo.FixedSampleLocations));
				}
			} else if constexpr (Type == TextureType::E2D_MULTISAMPLE_ARRAY) {
				if (GLAD_GL_VERSION_4_3 != 0 && allocInfo.Immutable) {
					glTexStorage3DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
						static_cast<GLsizei>(dimensions.y), static_cast<GLsizei>(dimensions.z),
						static_cast<GLboolean>(allocInfo.FixedSampleLocations));
				} else {
					glTexImage3DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
						static_cast<GLsizei>(dimensions.y), static_cast<GLsizei>(dimensions.z),
						static_cast<GLboolean>(allocInfo.FixedSampleLocations));
				}
			} else if constexpr (NUM_DIMENSIONS == 1) {
				if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
					glTexStorage1D(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.MipmapLevels),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x));
				} else {
					if (allocInfo.MipmapLevels != 1 && !detail::TextureSupportsMipmapping<Type>::value) {
						throw std::runtime_error("texture does not support mipmapping");
					}
					for (std::size_t i = 0; i < allocInfo.MipmapLevels; ++i) {
						glTexImage1D(static_cast<GLenum>(Type), static_cast<GLint>(i),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x), 0, GL_R,
							GL_UNSIGNED_BYTE, nullptr);
					}
				}
			} else if constexpr (NUM_DIMENSIONS == 2) {
				if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
					glTexStorage2D(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.MipmapLevels),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
						static_cast<GLsizei>(dimensions.y));
				} else {
					if (allocInfo.MipmapLevels != 1 && !detail::TextureSupportsMipmapping<Type>::value) {
						throw std::runtime_error("texture does not support mipmapping");
					}
					for (std::size_t i = 0; i < allocInfo.MipmapLevels; ++i) {
						glTexImage2D(static_cast<GLenum>(Type), static_cast<GLint>(i),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
							static_cast<GLsizei>(dimensions.y), 0, GL_R, GL_UNSIGNED_BYTE, nullptr);
					}
				}
			} else if constexpr (NUM_DIMENSIONS == 3) {
				if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
					glTexStorage3D(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.MipmapLevels),
						static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
						static_cast<GLsizei>(dimensions.y), static_cast<GLsizei>(dimensions.z));
				} else {
					if (allocInfo.MipmapLevels != 1 && !detail::TextureSupportsMipmapping<Type>::value) {
						throw std::runtime_error("texture does not support mipmapping");
					}
					for (std::size_t i = 0; i < allocInfo.MipmapLevels; ++i) {
						glTexImage3D(static_cast<GLenum>(Type), static_cast<GLint>(i),
							static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.x),
							static_cast<GLsizei>(dimensions.y), static_cast<GLsizei>(dimensions.z), 0, GL_R,
							GL_UNSIGNED_BYTE, nullptr);
					}
				}
			}
		}

		template <TextureType Type>
		inline auto PossiblyBoundTexture<Type>::imageCopy(
			const VectorType& offset, const DataType& data, std::size_t MipLevel) const -> void
		{
			if constexpr (NUM_DIMENSIONS == 1) {
				glTexSubImage1D(static_cast<GLenum>(Type), static_cast<GLint>(MipLevel), static_cast<GLint>(offset.x),
					static_cast<GLsizei>(data.Dimensions.x), static_cast<GLenum>(data.Format),
					static_cast<GLenum>(data.TexelType), data.Data);
			} else if constexpr (NUM_DIMENSIONS == 2) {
				glTexSubImage2D(static_cast<GLenum>(Type), static_cast<GLint>(MipLevel), static_cast<GLint>(offset.x),
					static_cast<GLint>(offset.y), static_cast<GLsizei>(data.Dimensions.x),
					static_cast<GLsizei>(data.Dimensions.y), static_cast<GLenum>(data.Format),
					static_cast<GLenum>(data.TexelType), data.Data);
			} else if constexpr (NUM_DIMENSIONS == 3) {
				glTexSubImage3D(static_cast<GLenum>(Type), static_cast<GLint>(MipLevel), static_cast<GLint>(offset.x),
					static_cast<GLint>(offset.y), static_cast<GLint>(offset.z), static_cast<GLsizei>(data.Dimensions.x),
					static_cast<GLsizei>(data.Dimensions.y), static_cast<GLsizei>(data.Dimensions.z),
					static_cast<GLenum>(data.Format), static_cast<GLenum>(data.TexelType), data.Data);
			}
		}

		template <TextureType Type>
		inline auto PossiblyBoundTexture<Type>::generateMipmap() const -> void
		{
			if (supportsDSA()) {
				glGenerateTextureMipmap(this->getHandle());
				return;
			}

			this->forceBind();
			glGenerateMipmap(static_cast<GLenum>(Type));
		}

		template <TextureType Type>
		inline auto Texture<Type>::imageClear(
			const VectorType& offset, const VectorType& dimensions, std::size_t mipLevel) const -> void
		{
			auto [x, y, z]				= padVectorTo3D<GLint>(offset);
			auto [width, height, depth] = padVectorTo3D<GLsizei>(dimensions, 1);
			std::uint32_t color			= 0;
			glClearTexSubImage(this->get(), static_cast<GLint>(mipLevel), x, y, z, width, height, depth, GL_RGBA,
				GL_UNSIGNED_BYTE, &color);
		}

		template <TextureType Type>
		inline auto Texture<Type>::invalidate(
			const VectorType& offset, const VectorType& dimensions, std::size_t mipLevel) const -> void
		{
			auto [x, y, z]				= padVectorTo3D<GLint>(offset);
			auto [width, height, depth] = padVectorTo3D<GLsizei>(dimensions, 1);
			glInvalidateTexSubImage(this->get(), static_cast<GLint>(mipLevel), x, y, z, width, height, depth);
		}

		template <TextureType SrcType>
		template <TextureType DestType>
		inline auto PossiblyBoundTexture<SrcType>::imageCopyToTexture(const PossiblyBoundTexture<DestType>& dest,
			const typename PossiblyBoundTexture<DestType>::VectorType& destOffset, const VectorType& srcOffset,
			const VectorType& srcDimensions, std::size_t destMipLevel, std::size_t srcMipLevel) const -> void
		{
			auto [srcX, srcY, srcZ]	   = Texture<SrcType>::template padVectorTo3D<GLint>(srcOffset);
			auto [destX, destY, destZ] = Texture<DestType>::template padVectorTo3D<GLint>(destOffset);
			auto [srcx, srcy, srcz]	   = Texture<SrcType>::template padVectorTo3D<GLint>(srcDimensions, 1);
			// just checked, there's no need to bind
			glCopyImageSubData(this->get(), static_cast<GLenum>(SrcType), static_cast<GLint>(srcMipLevel), srcX, srcY,
				srcZ, dest.get(), static_cast<GLenum>(DestType), static_cast<GLint>(destMipLevel), destX, destY, destZ,
				srcx, srcy, srcz);
		}

		// TODO: add buffer range checking
		template <TextureType Type>
		template <TextureType ThisType, typename>
		inline auto PossiblyBoundTexture<Type>::setStorageBuffer(
			TextureInternalFormat format, GLuint bufferHandle, std::size_t offset, std::size_t size) const -> void
		{
			static_assert(Type == ThisType);
			if (supportsDSA()) {
				if (offset == 0 && size == SIZE_MAX) {
					glTextureBuffer(this->getHandle(), static_cast<GLenum>(format), bufferHandle);
				} else {
					glTextureBufferRange(this->getHandle(), static_cast<GLenum>(format), bufferHandle, offset, size);
				}
			}

			this->forceBind();
			if (offset == 0 && size == SIZE_MAX) {
				glTexBuffer(static_cast<GLenum>(Type), static_cast<GLenum>(format), bufferHandle);
			} else {
				glTexBufferRange(static_cast<GLenum>(Type), static_cast<GLenum>(format), bufferHandle, offset, size);
			}
		}
		template <TextureType Type>
		template <typename T>
		inline auto Texture<Type>::padVectorTo3D(const VectorType& vector, T defaultValue) -> std::array<T, 3>
		{
			std::array<T, 3> out;
			out.fill(defaultValue);
			if constexpr (NUM_DIMENSIONS >= 1) {
				out[0] = vector.x;
			}
			if constexpr (NUM_DIMENSIONS >= 2) {
				out[1] = vector.y;
			}
			if constexpr (NUM_DIMENSIONS >= 3) {
				out[2] = vector.z;
			}
			return out;
		}

		template class Texture<TextureType::E2D>;
		template class Texture<TextureType::CUBE_MAP>;
		template class Texture<TextureType::E2D_MULTISAMPLE_ARRAY>;
	} // namespace Graphics
} // namespace HJUIK

#include <cstdint>
#include <stdexcept>

#include "Texture.hpp"

namespace HJUIK
{
  namespace Graphics
  {

    auto detail::TextureTrait::create() -> GLuint
    {
      return checkNonZero(callGLGen<GLuint>(glGenTextures), "unable to create texture");
    }

    auto detail::TextureTrait::destroy(GLuint handle) -> void
    {
      glDeleteTextures(1, &handle);
    }

    inline auto detail::getTextureBindingType(TextureType type) -> GLenum
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
    inline auto TextureDimension<Type>::toGLsizeiTuple() const -> std::tuple<GLsizei, GLsizei, GLsizei>
    {
      GLsizei width = 1, height = 1, depth = 1;
      if constexpr (detail::AT_LEAST_DIMENSION<Type, 1>) {
        width = static_cast<GLsizei>(this->Width);
      }
      if constexpr (detail::AT_LEAST_DIMENSION<Type, 2>) {
        height = static_cast<GLsizei>(this->Height);
      }
      if constexpr (detail::AT_LEAST_DIMENSION<Type, 3>) {
        depth = static_cast<GLsizei>(this->Depth);
      }

      return std::make_tuple(width, height, depth);
    }

    template <TextureType Type>
    inline auto TextureOffset<Type>::toGLintTuple() const -> std::tuple<GLint, GLint, GLint>
    {
      GLint x = 1, y = 1, z = 1;
      if constexpr (detail::AT_LEAST_DIMENSION<Type, 1>) {
        x = static_cast<GLint>(this->OffsetX);
      }
      if constexpr (detail::AT_LEAST_DIMENSION<Type, 2>) {
        y = static_cast<GLint>(this->OffsetY);
      }
      if constexpr (detail::AT_LEAST_DIMENSION<Type, 3>) {
        z = static_cast<GLint>(this->OffsetZ);
      }

      return std::make_tuple(x, y, z);
    }

    template <TextureType Type>
    inline auto Texture<Type>::bind() const -> void
    {
      glBindTexture(static_cast<GLenum>(Type), get());
    }

    template <TextureType Type>
    inline auto Texture<Type>::unbind() -> void
    {
      glBindTexture(static_cast<GLenum>(Type), 0);
    }

    template <TextureType Type>
    inline auto Texture<Type>::getCurrentBound() -> GLuint
    {
      return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, detail::getTextureBindingType(Type)));
    }

    template <TextureType Type>
    inline auto Texture<Type>::getDimension(std::size_t mipLevel) const -> DimensionType
    {
      DimensionType dimensions;
      BindGuard guard{*this};
      if constexpr (NUM_DIMENSIONS >= 1) {
        dimensions.Width = static_cast<std::size_t>(callGLGet<GLint>(
            glGetTexLevelParameteriv, static_cast<GLenum>(Type), static_cast<GLint>(mipLevel), GL_TEXTURE_WIDTH));
      }
      if constexpr (NUM_DIMENSIONS >= 2) {
        dimensions.Height = static_cast<std::size_t>(callGLGet<GLint>(
            glGetTexLevelParameteriv, static_cast<GLenum>(Type), static_cast<GLint>(mipLevel), GL_TEXTURE_HEIGHT));
      }
      if constexpr (NUM_DIMENSIONS >= 3) {
        dimensions.Depth = static_cast<std::size_t>(callGLGet<GLint>(
            glGetTexLevelParameteriv, static_cast<GLenum>(Type), static_cast<GLint>(mipLevel), GL_TEXTURE_DEPTH));
      }
      return dimensions;
    }

    template <TextureType Type>
    inline auto Texture<Type>::setLabel(const char* name) -> void
    {
      if (GLAD_GL_VERSION_4_3 != 0) {
        BindGuard guard{*this};
        glObjectLabel(GL_TEXTURE, get(), -1, name);
      }
    }

    template <TextureType Type>
    inline auto Texture<Type>::bindActive(std::size_t slot) const -> void
    {
      glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + slot));
      bind();
    }

    template <TextureType Type>
    inline auto Texture<Type>::allocate(const TextureAllocationInfo& allocInfo, const DimensionType& dimensions) -> void
    {
      if constexpr (Type == TextureType::E2D_MULTISAMPLE) {
        if (GLAD_GL_VERSION_4_3 != 0 && allocInfo.Immutable) {
          glTexStorage2DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
              static_cast<GLsizei>(dimensions.Height), static_cast<GLboolean>(allocInfo.FixedSampleLocations));
        } else {
          glTexImage2DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
              static_cast<GLsizei>(dimensions.Height), static_cast<GLboolean>(allocInfo.FixedSampleLocations));
        }
      } else if constexpr (Type == TextureType::E2D_MULTISAMPLE_ARRAY) {
        if (GLAD_GL_VERSION_4_3 != 0 && allocInfo.Immutable) {
          glTexStorage3DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
              static_cast<GLsizei>(dimensions.Height), static_cast<GLsizei>(dimensions.Depth),
              static_cast<GLboolean>(allocInfo.FixedSampleLocations));
        } else {
          glTexImage3DMultisample(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.Samples),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
              static_cast<GLsizei>(dimensions.Height), static_cast<GLsizei>(dimensions.Depth),
              static_cast<GLboolean>(allocInfo.FixedSampleLocations));
        }
      } else if constexpr (NUM_DIMENSIONS == 1) {
        if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
          glTexStorage1D(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.MipmapLevels),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width));
        } else {
          if (allocInfo.MipmapLevels != 1 && !detail::TextureSupportsMipmapping<Type>::value) {
            throw std::runtime_error("texture does not support mipmapping");
          }
          for (std::size_t i = 0; i < allocInfo.MipmapLevels; ++i) {
            glTexImage1D(static_cast<GLenum>(Type), static_cast<GLint>(i),
                static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width), 0, GL_R,
                GL_UNSIGNED_BYTE, nullptr);
          }
        }
      } else if constexpr (NUM_DIMENSIONS == 2) {
        if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
          glTexStorage2D(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.MipmapLevels),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
              static_cast<GLsizei>(dimensions.Height));
        } else {
          if (allocInfo.MipmapLevels != 1 && !detail::TextureSupportsMipmapping<Type>::value) {
            throw std::runtime_error("texture does not support mipmapping");
          }
          for (std::size_t i = 0; i < allocInfo.MipmapLevels; ++i) {
            glTexImage2D(static_cast<GLenum>(Type), static_cast<GLint>(i),
                static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
                static_cast<GLsizei>(dimensions.Height), 0, GL_R, GL_UNSIGNED_BYTE, nullptr);
          }
        }
      } else if constexpr (NUM_DIMENSIONS == 3) {
        if (GLAD_GL_VERSION_4_2 != 0 && allocInfo.Immutable) {
          glTexStorage3D(static_cast<GLenum>(Type), static_cast<GLsizei>(allocInfo.MipmapLevels),
              static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
              static_cast<GLsizei>(dimensions.Height), static_cast<GLsizei>(dimensions.Depth));
        } else {
          if (allocInfo.MipmapLevels != 1 && !detail::TextureSupportsMipmapping<Type>::value) {
            throw std::runtime_error("texture does not support mipmapping");
          }
          for (std::size_t i = 0; i < allocInfo.MipmapLevels; ++i) {
            glTexImage3D(static_cast<GLenum>(Type), static_cast<GLint>(i),
                static_cast<GLenum>(allocInfo.InternalFormat), static_cast<GLsizei>(dimensions.Width),
                static_cast<GLsizei>(dimensions.Height), static_cast<GLsizei>(dimensions.Depth), 0, GL_R,
                GL_UNSIGNED_BYTE, nullptr);
          }
        }
      }
    }

    template <TextureType Type>
    inline auto Texture<Type>::imageCopy(const OffsetType& offset, const DataType& data, std::size_t MipLevel) -> void
    {
      if constexpr (NUM_DIMENSIONS == 1) {
        glTexSubImage1D(static_cast<GLenum>(Type), static_cast<GLint>(MipLevel), static_cast<GLint>(offset.OffsetX),
            static_cast<GLsizei>(data.Width), static_cast<GLenum>(data.Format), static_cast<GLenum>(data.TexelType),
            data.Data);
      } else if constexpr (NUM_DIMENSIONS == 2) {
        glTexSubImage2D(static_cast<GLenum>(Type), static_cast<GLint>(MipLevel), static_cast<GLint>(offset.OffsetX),
            static_cast<GLint>(offset.OffsetY), static_cast<GLsizei>(data.Width), static_cast<GLsizei>(data.Height),
            static_cast<GLenum>(data.Format), static_cast<GLenum>(data.TexelType), data.Data);
      } else if constexpr (NUM_DIMENSIONS == 3) {
        glTexSubImage3D(static_cast<GLenum>(Type), static_cast<GLint>(MipLevel), static_cast<GLint>(offset.OffsetX),
            static_cast<GLint>(offset.OffsetY), static_cast<GLint>(offset.OffsetZ), static_cast<GLsizei>(data.Width),
            static_cast<GLsizei>(data.Height), static_cast<GLsizei>(data.Depth), static_cast<GLenum>(data.Format),
            static_cast<GLenum>(data.TexelType), data.Data);
      }
    }

    template <TextureType Type>
    inline auto Texture<Type>::generateMipmap() -> void
    {
      glGenerateMipmap(static_cast<GLenum>(Type));
    }

    template <TextureType Type>
    inline auto Texture<Type>::imageClear(
        const OffsetType& offset, const DimensionType& dimension, std::size_t mipLevel) const -> void
    {
      auto [x, y, z]              = offset.toGLintTuple();
      auto [width, height, depth] = dimension.toGLsizeiTuple();
      std::uint32_t color         = 0;
      glClearTexSubImage(
          get(), static_cast<GLint>(mipLevel), x, y, z, width, height, depth, GL_RGBA, GL_UNSIGNED_BYTE, &color);
    }

    template <TextureType Type>
    inline auto Texture<Type>::invalidate(
        const OffsetType& offset, const DimensionType& dimension, std::size_t mipLevel) const -> void
    {
      auto [x, y, z]              = offset.toGLintTuple();
      auto [width, height, depth] = dimension.toGLsizeiTuple();
      glInvalidateTexSubImage(get(), static_cast<GLint>(mipLevel), x, y, z, width, height, depth);
    }

    template <TextureType SrcType>
    template <TextureType DestType>
    inline auto Texture<SrcType>::imageCopyToTexture(const Texture<DestType>& dest,
        const TextureOffset<DestType>& destOffset, const OffsetType& srcOffset, const DimensionType& srcDimension,
        std::size_t destMipLevel, std::size_t srcMipLevel) const -> void
    {
      auto [srcX, srcY, srcZ]              = srcOffset.toGLintTuple();
      auto [destX, destY, destZ]           = destOffset.toGLintTuple();
      auto [srcWidth, srcHeight, srcDepth] = srcDimension.toGLsizeiTuple();
      glCopyImageSubData(get(), static_cast<GLenum>(SrcType), static_cast<GLint>(srcMipLevel), srcX, srcY, srcZ,
          dest.get(), static_cast<GLenum>(DestType), static_cast<GLint>(destMipLevel), destX, destY, destZ, srcWidth,
          srcHeight, srcDepth);
    }

    // TODO: add buffer range checking
    template <TextureType Type>
    template <TextureType ThisType, typename>
    inline auto Texture<Type>::setStorageBuffer(
        TextureInternalFormat format, GLuint bufferHandle, std::size_t offset, std::size_t size) const -> void
    {
      static_assert(Type == ThisType);
      if (offset == 0 && size == SIZE_MAX) {
        glTextureBuffer(get(), static_cast<GLenum>(format), bufferHandle);
      } else {
        glTextureBufferRange(get(), static_cast<GLenum>(format), bufferHandle, offset, size);
      }
    }
  } // namespace Graphics
} // namespace HJUIK

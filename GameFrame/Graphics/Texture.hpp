#ifndef GAMEFRAME_GRAPHICS_TEXTURE_HPP
#define GAMEFRAME_GRAPHICS_TEXTURE_HPP

#include "glad/glad.h"
#include <type_traits>
#include <tuple>

#include "OpenGLWrapper.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    enum class TextureType {
      E1D = GL_TEXTURE_1D,
      E2D = GL_TEXTURE_2D,
      E3D = GL_TEXTURE_3D,

      RECTANGLE = GL_TEXTURE_RECTANGLE,
      CUBE_MAP  = GL_TEXTURE_CUBE_MAP,
      BUFFER    = GL_TEXTURE_BUFFER,

      E1D_ARRAY      = GL_TEXTURE_1D_ARRAY,
      E2D_ARRAY      = GL_TEXTURE_2D_ARRAY,
      CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,

      E2D_MULTISAMPLE       = GL_TEXTURE_2D_MULTISAMPLE,
      E2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
    };

    namespace detail
    {
      struct TextureTrait {
        using HandleType = GLuint;
        static auto create() -> GLuint;
        static auto destroy(GLuint handle) -> void;
      };

      inline auto getTextureBindingType(TextureType type) -> GLenum;

      template <TextureType Type>
      struct TextureDimensionType : std::integral_constant<size_t, 0> {
      };

      template <TextureType Type>
      struct TextureSupportsMipmapping : std::false_type {
      };

      template <TextureType Type>
      struct TextureIsArray : std::false_type {
      };

      template <TextureType Type>
      struct TextureIsCubeMap : std::false_type {
      };

      // clang-format off
      template <> struct TextureDimensionType<TextureType::E1D>                   : std::integral_constant<size_t, 1> {};
      template <> struct TextureDimensionType<TextureType::E2D>                   : std::integral_constant<size_t, 2> {};
      template <> struct TextureDimensionType<TextureType::E3D>                   : std::integral_constant<size_t, 3> {};
      template <> struct TextureDimensionType<TextureType::RECTANGLE>             : std::integral_constant<size_t, 2> {};
      template <> struct TextureDimensionType<TextureType::CUBE_MAP>              : std::integral_constant<size_t, 3> {};
      template <> struct TextureDimensionType<TextureType::BUFFER>                : std::integral_constant<size_t, 1> {};
      template <> struct TextureDimensionType<TextureType::E1D_ARRAY>             : std::integral_constant<size_t, 2> {};
      template <> struct TextureDimensionType<TextureType::E2D_ARRAY>             : std::integral_constant<size_t, 3> {};
      template <> struct TextureDimensionType<TextureType::CUBE_MAP_ARRAY>        : std::integral_constant<size_t, 3> {};
      template <> struct TextureDimensionType<TextureType::E2D_MULTISAMPLE>       : std::integral_constant<size_t, 2> {};
      template <> struct TextureDimensionType<TextureType::E2D_MULTISAMPLE_ARRAY> : std::integral_constant<size_t, 3> {};

      template <> struct TextureSupportsMipmapping<TextureType::E1D>              : std::true_type {};
      template <> struct TextureSupportsMipmapping<TextureType::E2D>              : std::true_type {};
      template <> struct TextureSupportsMipmapping<TextureType::E3D>              : std::true_type {};
      template <> struct TextureSupportsMipmapping<TextureType::E1D_ARRAY>        : std::true_type {};
      template <> struct TextureSupportsMipmapping<TextureType::E2D_ARRAY>        : std::true_type {};
      template <> struct TextureSupportsMipmapping<TextureType::CUBE_MAP>         : std::true_type {};
      template <> struct TextureSupportsMipmapping<TextureType::CUBE_MAP_ARRAY>   : std::true_type {};

      template <> struct TextureIsArray<TextureType::E1D_ARRAY>                   : std::true_type {};
      template <> struct TextureIsArray<TextureType::E2D_ARRAY>                   : std::true_type {};
      template <> struct TextureIsArray<TextureType::E2D_MULTISAMPLE_ARRAY>       : std::true_type {};
      template <> struct TextureIsArray<TextureType::CUBE_MAP_ARRAY>              : std::true_type {};

      template <> struct TextureIsCubeMap<TextureType::CUBE_MAP>                  : std::true_type {};
      template <> struct TextureIsCubeMap<TextureType::CUBE_MAP_ARRAY>            : std::true_type {};

      // clang-format on

      template <TextureType Type>
      inline constexpr size_t TEXTURE_DIMENSION = TextureDimensionType<Type>::value;

      template <TextureType Type, size_t Dimension>
      inline constexpr bool AT_LEAST_DIMENSION = TEXTURE_DIMENSION<Type> >= Dimension;

      struct TextureSizeAtLeast1D {
        size_t Width = 0;
      };

      struct TextureSizeAtLeast2D {
        size_t Height = 0;
      };

      struct TextureSizeAtLeast3D {
        size_t Depth = 0;
      };

      struct TextureOffsetAtLeast1D {
        size_t OffsetX = 0;
      };

      struct TextureOffsetAtLeast2D {
        size_t OffsetY = 0;
      };

      struct TextureOffsetAtLeast3D {
        size_t OffsetZ = 0;
      };

      struct Empty {
      };
    } // namespace detail

    enum class TextureTexelType {
      UNSIGNED_BYTE               = GL_UNSIGNED_BYTE,
      BYTE                        = GL_BYTE,
      UNSIGNED_SHORT              = GL_UNSIGNED_SHORT,
      SHORT                       = GL_SHORT,
      UNSIGNED_INT                = GL_UNSIGNED_INT,
      INT                         = GL_INT,
      FLOAT                       = GL_FLOAT,
      UNSIGNED_BYTE_3_3_2         = GL_UNSIGNED_BYTE_3_3_2,
      UNSIGNED_BYTE_2_3_3_REV     = GL_UNSIGNED_BYTE_2_3_3_REV,
      UNSIGNED_SHORT_5_6_5        = GL_UNSIGNED_SHORT_5_6_5,
      UNSIGNED_SHORT_5_6_5_REV    = GL_UNSIGNED_SHORT_5_6_5_REV,
      UNSIGNED_SHORT_4_4_4_4      = GL_UNSIGNED_SHORT_4_4_4_4,
      UNSIGNED_SHORT_4_4_4_4_REV  = GL_UNSIGNED_SHORT_4_4_4_4_REV,
      UNSIGNED_SHORT_5_5_5_1      = GL_UNSIGNED_SHORT_5_5_5_1,
      UNSIGNED_SHORT_1_5_5_5_REV  = GL_UNSIGNED_SHORT_1_5_5_5_REV,
      UNSIGNED_INT_8_8_8_8        = GL_UNSIGNED_INT_8_8_8_8,
      UNSIGNED_INT_8_8_8_8_REV    = GL_UNSIGNED_INT_8_8_8_8_REV,
      UNSIGNED_INT_10_10_10_2     = GL_UNSIGNED_INT_10_10_10_2,
      UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
    };

    enum class TextureFormat {
      RED             = GL_RED,
      RED_GREEN       = GL_RG,
      RGB             = GL_RGB,
      BGR             = GL_BGR,
      RGBA            = GL_RGBA,
      DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
      STENCIL_INDEX   = GL_STENCIL_INDEX,
    };

    class TextureInternalFormat
    {
    public:
      constexpr explicit TextureInternalFormat(GLenum value) : mValue{value} {}

      constexpr auto get() const -> GLenum
      {
        return mValue;
      }

      constexpr explicit operator GLenum() const
      {
        return get();
      }

    private:
      GLenum mValue;
    };

    template <TextureType Type>
    struct TextureDimension
        : std::conditional_t<detail::AT_LEAST_DIMENSION<Type, 1>, detail::TextureSizeAtLeast1D, detail::Empty>,
          std::conditional_t<detail::AT_LEAST_DIMENSION<Type, 2>, detail::TextureSizeAtLeast2D, detail::Empty>,
          std::conditional_t<detail::AT_LEAST_DIMENSION<Type, 3>, detail::TextureSizeAtLeast3D, detail::Empty> {
      auto toGLsizeiTuple() const -> std::tuple<GLsizei, GLsizei, GLsizei>;
    };

    template <TextureType Type>
    struct TextureOffset
        : std::conditional_t<detail::AT_LEAST_DIMENSION<Type, 1>, detail::TextureOffsetAtLeast1D, detail::Empty>,
          std::conditional_t<detail::AT_LEAST_DIMENSION<Type, 2>, detail::TextureOffsetAtLeast2D, detail::Empty>,
          std::conditional_t<detail::AT_LEAST_DIMENSION<Type, 3>, detail::TextureOffsetAtLeast3D, detail::Empty> {
      auto toGLintTuple() const -> std::tuple<GLint, GLint, GLint>;
    };

    template <TextureType Type>
    struct TextureData : TextureDimension<Type> {
      const void* Data;
      TextureFormat Format;
      TextureTexelType TexelType;
    };

    namespace TextureInternalFormats
    {
      constexpr static TextureInternalFormat RGBA8{GL_RGBA8};
      constexpr static TextureInternalFormat RGB8{GL_RGB8};
      constexpr static TextureInternalFormat RG8{GL_RG8};
      constexpr static TextureInternalFormat RED_8{GL_R8};
      constexpr static TextureInternalFormat RGBA32F{GL_RGBA32F};
      constexpr static TextureInternalFormat RGB32F{GL_RGB32F};
      constexpr static TextureInternalFormat RG32F{GL_RG32F};
      constexpr static TextureInternalFormat R32F{GL_R32F};
      // TODO: add more
    } // namespace TextureInternalFormats


    struct TextureAllocationInfo {
      // similar to the `Immutable` in `BufferUsage`
      // immutable data means that the size of the texture will not change,
      // not the texel data could not be modified
      bool Immutable                       = true;
      size_t MipmapLevels                  = 1;
      TextureInternalFormat InternalFormat = TextureInternalFormats::RGBA8;
      // For E2D_MULTISAMPLE/E2D_MULTISAMPLE_ARRAY types only
      size_t Samples            = 0;
      bool FixedSampleLocations = false;
    };

    template <TextureType Type>
    class Texture : public OpenGLWrapper<detail::TextureTrait>
    {
    public:
      using OpenGLWrapper::OpenGLWrapper;
      using OpenGLWrapper::operator=;
      constexpr static std::size_t NUM_DIMENSIONS = detail::TEXTURE_DIMENSION<Type>;
      using DimensionType                         = TextureDimension<Type>;
      using OffsetType                            = TextureOffset<Type>;
      using DataType                              = TextureData<Type>;

      auto bind() const -> void;
      static auto unbind() -> void;

      static auto getCurrentBound() -> GLuint;

      auto getDimension(size_t mipLevel = 0) const -> DimensionType;

      auto setLabel(const char* name) -> void;

      auto bindActive(size_t slot) const -> void;

      static auto allocate(const TextureAllocationInfo& allocInfo, const DimensionType& dimensions) -> void;
      static auto imageCopy(const OffsetType& offset, const DataType& data, size_t MipLevel = 0) -> void;
      static auto generateMipmap() -> void;
      // TODO: add custom clear
      auto imageClear(const OffsetType& offset, const DimensionType& dimension, size_t mipLevel = 0) const -> void;

      template <TextureType DestType>
      auto imageCopyToTexture(const Texture<DestType>& dest, const TextureOffset<DestType>& destOffset,
          const OffsetType& srcOffset, const DimensionType& srcDimension, size_t destMipLevel = 0,
          size_t srcMipLevel = 0) const -> void;

      auto invalidate(const OffsetType& offset, const DimensionType& dimension, size_t mipLevel = 0) const -> void;

      template <TextureType ThisType = Type, typename = std::enable_if_t<(ThisType == TextureType::BUFFER)>>
      auto setStorageBuffer(
          TextureInternalFormat format, GLuint bufferHandle, size_t offset = 0, size_t size = SIZE_MAX) const -> void;
    };

    template class Texture<TextureType::CUBE_MAP>;
    template class Texture<TextureType::CUBE_MAP_ARRAY>;
  } // namespace Graphics
} // namespace HJUIK

#include "Texture.inl"

#endif

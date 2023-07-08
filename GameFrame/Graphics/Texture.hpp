#ifndef GAMEFRAME_GRAPHICS_TEXTURE_HPP
#define GAMEFRAME_GRAPHICS_TEXTURE_HPP

#include "deps/glad/glad.h"
#include <array>
#include <type_traits>

#include "../Utilize/GLMTypedef.hpp"
#include "ImageLoader.hpp"
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
			BUFFER	  = GL_TEXTURE_BUFFER,

			E1D_ARRAY	   = GL_TEXTURE_1D_ARRAY,
			E2D_ARRAY	   = GL_TEXTURE_2D_ARRAY,
			CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,

			E2D_MULTISAMPLE		  = GL_TEXTURE_2D_MULTISAMPLE,
			E2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		};

		namespace detail
		{
			template <TextureType Type>
			struct TextureTrait {
				using HandleType = GLuint;
				static auto create() -> GLuint;
				static auto destroy(GLuint handle) -> void;

				static auto getCurrentBound() -> GLuint;
				static auto bind(GLuint handle) -> void;
			};

			inline constexpr auto getTextureBindingType(TextureType type) -> GLenum;

			template <TextureType Type>
			struct TextureDimensionsType : std::integral_constant<std::size_t, 0> {
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
      template <> struct TextureDimensionsType<TextureType::E1D>                   : std::integral_constant<std::size_t, 1> {};
      template <> struct TextureDimensionsType<TextureType::E2D>                   : std::integral_constant<std::size_t, 2> {};
      template <> struct TextureDimensionsType<TextureType::E3D>                   : std::integral_constant<std::size_t, 3> {};
      template <> struct TextureDimensionsType<TextureType::RECTANGLE>             : std::integral_constant<std::size_t, 2> {};
      template <> struct TextureDimensionsType<TextureType::CUBE_MAP>              : std::integral_constant<std::size_t, 3> {};
      template <> struct TextureDimensionsType<TextureType::BUFFER>                : std::integral_constant<std::size_t, 1> {};
      template <> struct TextureDimensionsType<TextureType::E1D_ARRAY>             : std::integral_constant<std::size_t, 2> {};
      template <> struct TextureDimensionsType<TextureType::E2D_ARRAY>             : std::integral_constant<std::size_t, 3> {};
      template <> struct TextureDimensionsType<TextureType::CUBE_MAP_ARRAY>        : std::integral_constant<std::size_t, 3> {};
      template <> struct TextureDimensionsType<TextureType::E2D_MULTISAMPLE>       : std::integral_constant<std::size_t, 2> {};
      template <> struct TextureDimensionsType<TextureType::E2D_MULTISAMPLE_ARRAY> : std::integral_constant<std::size_t, 3> {};

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
			inline constexpr std::size_t TEXTURE_DIMENSIONS = TextureDimensionsType<Type>::value;

			template <TextureType Type, std::size_t Dimensions>
			inline constexpr bool AT_LEAST_DIMENSIONS = TEXTURE_DIMENSIONS<Type> >= Dimensions;
		} // namespace detail

		enum class TextureTexelType {
			UNSIGNED_BYTE				= GL_UNSIGNED_BYTE,
			BYTE						= GL_BYTE,
			UNSIGNED_SHORT				= GL_UNSIGNED_SHORT,
			SHORT						= GL_SHORT,
			UNSIGNED_INT				= GL_UNSIGNED_INT,
			INT							= GL_INT,
			FLOAT						= GL_FLOAT,
			UNSIGNED_BYTE_3_3_2			= GL_UNSIGNED_BYTE_3_3_2,
			UNSIGNED_BYTE_2_3_3_REV		= GL_UNSIGNED_BYTE_2_3_3_REV,
			UNSIGNED_SHORT_5_6_5		= GL_UNSIGNED_SHORT_5_6_5,
			UNSIGNED_SHORT_5_6_5_REV	= GL_UNSIGNED_SHORT_5_6_5_REV,
			UNSIGNED_SHORT_4_4_4_4		= GL_UNSIGNED_SHORT_4_4_4_4,
			UNSIGNED_SHORT_4_4_4_4_REV	= GL_UNSIGNED_SHORT_4_4_4_4_REV,
			UNSIGNED_SHORT_5_5_5_1		= GL_UNSIGNED_SHORT_5_5_5_1,
			UNSIGNED_SHORT_1_5_5_5_REV	= GL_UNSIGNED_SHORT_1_5_5_5_REV,
			UNSIGNED_INT_8_8_8_8		= GL_UNSIGNED_INT_8_8_8_8,
			UNSIGNED_INT_8_8_8_8_REV	= GL_UNSIGNED_INT_8_8_8_8_REV,
			UNSIGNED_INT_10_10_10_2		= GL_UNSIGNED_INT_10_10_10_2,
			UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV,
		};

		enum class TextureFormat {
			RED				= GL_RED,
			RED_GREEN		= GL_RG,
			RGB				= GL_RGB,
			BGR				= GL_BGR,
			RGBA			= GL_RGBA,
			DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
			STENCIL_INDEX	= GL_STENCIL_INDEX,
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
		struct TextureData {
			const void* Data;
			glm::vec<detail::TEXTURE_DIMENSIONS<Type>, std::size_t> Dimensions;
			TextureFormat Format;
			TextureTexelType TexelType;
		};

		namespace TextureInternalFormats
		{
			inline constexpr TextureInternalFormat RGBA8{GL_RGBA8};
			inline constexpr TextureInternalFormat RGB8{GL_RGB8};
			inline constexpr TextureInternalFormat RG8{GL_RG8};
			inline constexpr TextureInternalFormat RED_8{GL_R8};
			inline constexpr TextureInternalFormat RGBA32F{GL_RGBA32F};
			inline constexpr TextureInternalFormat RGB32F{GL_RGB32F};
			inline constexpr TextureInternalFormat RG32F{GL_RG32F};
			inline constexpr TextureInternalFormat R32F{GL_R32F};
			// TODO: add more
		} // namespace TextureInternalFormats


		struct TextureAllocationInfo {
			// similar to the `Immutable` in `BufferUsage`
			// immutable data means that the size of the texture will not change,
			// not the texel data could not be modified
			bool Immutable						 = true;
			std::size_t MipmapLevels			 = 1;
			TextureInternalFormat InternalFormat = TextureInternalFormats::RGBA8;
			// For E2D_MULTISAMPLE/E2D_MULTISAMPLE_ARRAY types only
			std::size_t Samples		  = 0;
			bool FixedSampleLocations = false;
		};

		template <TextureType Type>
		class PossiblyBoundTexture : public PossiblyBoundOpenGLWrapper<detail::TextureTrait<Type>>
		{
			using Base = PossiblyBoundOpenGLWrapper<detail::TextureTrait<Type>>;

		public:
			// using PossiblyBoundOpenGLWrapper::PossiblyBoundOpenGLWrapper does not work here
			using Base::Base;
			using Base::operator=;
			constexpr static std::size_t NUM_DIMENSIONS = detail::TEXTURE_DIMENSIONS<Type>;
			using VectorType							= glm::vec<NUM_DIMENSIONS, std::size_t>;
			using DataType								= TextureData<Type>;

			// get the texture dimensions
			auto getDimensions(std::size_t mipLevel = 0) const -> VectorType;
			// texture memory-related methods
			// allocate texture image data
			auto allocate(const TextureAllocationInfo& allocInfo, const VectorType& dimensions) const -> void;
			// copy image from client (CPU) to server (GPU)
			auto imageCopy(const VectorType& offset, const DataType& data, std::size_t MipLevel = 0) const -> void;
			// generate mipmaps for the texture
			auto generateMipmap() const -> void;

			// copy image from this texture to another texture
			template <TextureType DestType>
			auto imageCopyToTexture(const PossiblyBoundTexture<DestType>& dest,
				const typename PossiblyBoundTexture<DestType>::VectorType& destOffset, const VectorType& srcOffset,
				const VectorType& srcDimensions, std::size_t destMipLevel = 0, std::size_t srcMipLevel = 0) const
				-> void;

			// set data buffer for this TextureBuffer
			template <TextureType ThisType = Type, typename = std::enable_if_t<(ThisType == TextureType::BUFFER)>>
			auto setStorageBuffer(TextureInternalFormat format, GLuint bufferHandle, std::size_t offset = 0,
				std::size_t size = SIZE_MAX) const -> void;
		};

		template <TextureType Type>
		class Texture : public OpenGLWrapper<detail::TextureTrait<Type>, PossiblyBoundTexture<Type>>
		{
			using Base = OpenGLWrapper<detail::TextureTrait<Type>, PossiblyBoundTexture<Type>>;

		public:
			// using PossiblyBoundOpenGLWrapper::PossiblyBoundOpenGLWrapper does not work here
			using Base::Base;
			using Base::operator=;
			constexpr static std::size_t NUM_DIMENSIONS = PossiblyBoundTexture<Type>::NUM_DIMENSIONS;
			using VectorType							= typename PossiblyBoundTexture<Type>::VectorType;
			using DataType								= typename PossiblyBoundTexture<Type>::DataType;

			// set a label for this Texture via `glObjectLabel`.
			// this label may show up in debug callback or an external OpenGL
			// debugger (e.g. RenderDoc)
			// (only have effect in OpenGL 4.3+)
			auto setLabel(const char* name) -> void;

			// bind a texture to the texture slot `slot`
			// since this calls glBindTexture, it returns a `BoundTexture`
			// it's okay to discard this btw
			auto bindActive(std::size_t slot) const -> PossiblyBoundTexture<Type>;

			// TODO: add custom clear
			// clear the texture to a default color of black transparent
			// (this default color may be changed in the future)
			auto imageClear(const VectorType& offset, const VectorType& dimensions, std::size_t mipLevel = 0) const
				-> void;

			// invalidate texture, i.e., marking the texture data (in the specified region) as invalid
			auto invalidate(const VectorType& offset, const VectorType& dimensions, std::size_t mipLevel = 0) const
				-> void;

		private:
			template <typename T>
			static auto padVectorTo3D(const VectorType& vector, T defaultValue = 0) -> std::array<T, 3>;
			friend class PossiblyBoundTexture<Type>;
		};

		// only aliasing the most frequently used texture types
		using Texture2D		 = Texture<TextureType::E2D>;
		using Texture2DArray = Texture<TextureType::E2D_ARRAY>;
		using TextureCubeMap = Texture<TextureType::CUBE_MAP>;

		auto load2DTexture(const Image2DLoader::RawData& data) -> Texture2D;
	} // namespace Graphics
} // namespace HJUIK

#include "Texture.inl"

#endif

#include "Texture.hpp"

#include <utility>

auto HJUIK::Graphics::load2DTexture(const Image2DLoader::RawData& data) -> Texture2D
{
	const auto&& [alignment, internalFormat, format, swizzleMask] = [&] {
		std::array<GLint, 4> swizzleMask{};
		switch (data.Format) {
		case ImageFormat::GRAY:
			swizzleMask[0] = GL_RED;
			swizzleMask[1] = GL_RED;
			swizzleMask[2] = GL_RED;
			swizzleMask[3] = GL_ONE;
			return std::make_tuple(1, TextureInternalFormats::RED_8, TextureFormat::RED, swizzleMask);
		case ImageFormat::GRAY_ALPHA:
			swizzleMask[0] = GL_RED;
			swizzleMask[1] = GL_RED;
			swizzleMask[2] = GL_RED;
			swizzleMask[3] = GL_GREEN;
			return std::make_tuple(2, TextureInternalFormats::RG8, TextureFormat::RED_GREEN, swizzleMask);
		case ImageFormat::RGB:
			swizzleMask[0] = GL_RED;
			swizzleMask[1] = GL_GREEN;
			swizzleMask[2] = GL_BLUE;
			swizzleMask[3] = GL_ONE;
			return std::make_tuple(1, TextureInternalFormats::RGB8, TextureFormat::RGB, swizzleMask);
		case ImageFormat::RGBA:
			swizzleMask[0] = GL_RED;
			swizzleMask[1] = GL_GREEN;
			swizzleMask[2] = GL_BLUE;
			swizzleMask[3] = GL_ALPHA;
			return std::make_tuple(4, TextureInternalFormats::RGBA8, TextureFormat::RGBA, swizzleMask);
		}

		HJUIK_ASSERT(false, "invalid image format");
	}();

	Texture2D texture;
	auto boundTexture	  = texture.bind();
	const auto dimensions = glm::vec<2, size_t>{data.Dimensions.x, data.Dimensions.y};

	TextureAllocationInfo allocInfo{};
	allocInfo.InternalFormat = internalFormat;
	boundTexture.allocate(allocInfo, dimensions);

	TextureData<TextureType::E2D> textureData{};
	textureData.Data	   = static_cast<const void*>(data.Value.get());
	textureData.Dimensions = dimensions;
	textureData.Format	   = format;
	textureData.TexelType  = TextureTexelType::UNSIGNED_BYTE;
	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
	boundTexture.imageCopy(glm::vec<2, size_t>{0, 0}, textureData);
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask.data());

	return std::move(texture);
}

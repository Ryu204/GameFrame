#ifndef GAMEFRAME_GRAPHICS_IMAGE_LOADER_HPP
#define GAMEFRAME_GRAPHICS_IMAGE_LOADER_HPP

/*
    Static class for loading a 2D image from files.
*/

#include <filesystem>
#include <cstdint>
#include <vector>
#include "deps/glad/glad.h"
#include "../Utilize/GLMTypedef.hpp"

namespace HJUIK
{
    namespace Graphics
    {
		enum class ImageFormat // != Image extension
		{
			R = GL_R,
            RG = GL_RG,
            RGB = GL_RGB,
            RGBA = GL_RGBA
			// I don't know why we would need more than this
		};

        namespace detail
        {
			auto imageFormatToNum(ImageFormat format) -> std::size_t;
			auto numToImageFormat(std::size_t num) -> ImageFormat;
		} // namespace detail

		class Image2DLoader
		{
		public:
			static constexpr Vector2u DEFAULT_DIMENSION{10, 10};
			static constexpr ImageFormat DEFAULT_FORMAT{ImageFormat::RGBA};

            // NOLINTBEGIN(*-member-init)
			struct Data
            {
				std::vector<std::uint8_t> Value;
				Vector2u Dimensions;
				ImageFormat Format;
			};
			// NOLINTEND(*-member-init)

			// load the image from 'filename' and return its information including color array
            // if the load failed, set value of `succeeded()` and return default image data
			static auto loadFromFile(const std::string& filename) -> Data;
            // query the latest `loadFromFile()` status
			static auto succeeded() -> bool;
            // create a white image data, can be used in case of load failure
			static auto create(Vector2u dimensions = DEFAULT_DIMENSION, ImageFormat format = DEFAULT_FORMAT) -> Data;
        private:
            static auto mLogInfo() -> std::string&;
			static auto mInternalStatus() -> bool&;
			Image2DLoader() = default;
		};
	} // namespace Graphics
} // namespace HJUIK

#endif
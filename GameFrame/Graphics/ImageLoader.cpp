#include "ImageLoader.hpp"

#include "../Utilize/CallAssert.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"
#include <algorithm>
#include <iterator>
#include <utility>

namespace HJUIK
{
	namespace Graphics
	{
		namespace detail
		{
			auto imageFormatToNum(ImageFormat format) -> std::size_t
			{
				return static_cast<std::size_t>(format);
			}

			auto numToImageFormat(std::size_t num) -> ImageFormat
			{
				return static_cast<ImageFormat>(num);
			}
		} // namespace detail

		// load the image from 'filename' and return its information including color array
		auto Image2DLoader::loadFromFile(const std::string& filename, bool throwAtFail) -> RawData
		{
			int width	   = 0;
			int height	   = 0;
			int channelNum = 0;
			// No longer encourage RGBA format
			// NOLINTBEGIN(clang-analyzer-unix.Malloc)
			// NOLINTNEXTLINE(*-avoid-c-arrays)
			std::unique_ptr<uint8_t[]> data{stbi_load(filename.c_str(), &width, &height, &channelNum, 0)};

			if (data == nullptr || width <= 0 || height <= 0 || channelNum <= 0 || channelNum > 4) {
				("HJUIK: stb_image.h: " + std::string(stbi_failure_reason())).swap(mErrLog());
				mInternalStatus() = false;
				if (throwAtFail) {
					throw std::runtime_error(mErrLog());
				}
				return create();
			}
			// NOLINTEND(clang-analyzer-unix.Malloc)

			// Now we are sure the load is successful
			RawData result;
			result.Dimensions = Vector2u{width, height};
			result.Format	  = detail::numToImageFormat(channelNum);
			result.Value	  = std::move(data);
			// Clean up
			mInternalStatus() = true;
			return result;
		}

		// query the latest loadFromFile() status
		auto Image2DLoader::succeeded() -> bool
		{
			return mInternalStatus();
		}

		// query the error if last `loadFromFile()` failed
		auto Image2DLoader::getErrLog() -> const std::string&
		{
			HJUIK_ASSERT(!succeeded(), "Last operation was successful");
			return mErrLog();
		}

		// create a white image data, can be used in case of load failure
		auto Image2DLoader::create(Vector2u dimensions, ImageFormat format) -> RawData
		{
			const auto size = detail::imageFormatToNum(format) * dimensions.x * dimensions.y;
			// NOLINTNEXTLINE(*-avoid-c-arrays)
			auto data = std::make_unique<uint8_t[]>(size);
			std::fill_n(data.get(), size, UINT8_MAX);
			return RawData{std::move(data), dimensions, format};
		}

		auto Image2DLoader::mInternalStatus() -> bool&
		{
			static bool status = false;
			return status;
		}

		auto Image2DLoader::mErrLog() -> std::string&
		{
			static std::string log;
			return log;
		}
	} // namespace Graphics
} // namespace HJUIK

#include "Texture2DAsset.hpp"

#include <utility>

namespace HJUIK
{
	namespace Assets
	{
		auto detail::ImageData::load() -> Graphics::Image2DLoader::RawData
		{
			return std::move(Data);
		}

		auto detail::ImageData::registerUpdateHandler(const std::function<void()>& callback) -> void {}

		auto detail::ImageFile::load() -> Graphics::Image2DLoader::RawData
		{
			return File->loadImage();
		}

		auto detail::ImageFile::registerUpdateHandler(std::function<void()> callback) -> void
		{
			File->registerUpdateHandler(std::move(callback));
		}

		Texture2DAsset::Texture2DAsset(Graphics::Image2DLoader::RawData data)
			: mImage{detail::ImageData{std::move(data)}}
		{
			load();
		}

		Texture2DAsset::Texture2DAsset(std::shared_ptr<FileAsset> file) : mImage{detail::ImageFile{std::move(file)}}
		{
			load();
		}

		Texture2DAsset::Texture2DAsset(Path path) : Texture2DAsset{std::make_shared<FileAsset>(std::move(path))} {}

		Texture2DAsset::~Texture2DAsset() = default;

		auto Texture2DAsset::get() -> Texture2DAssetValue
		{
			const auto wasInvalidated = mInvalidated.load(std::memory_order_relaxed);
			if (wasInvalidated) {
				mTexture = load();
				if (!Graphics::Image2DLoader::succeeded()) {
					std::cout << "error while hot-reloading texture: " << Graphics::Image2DLoader::getErrLog() << '\n';
				}

				assetUpdated();
				mInvalidated.store(false, std::memory_order_relaxed);
			}
			return Texture2DAssetValue{
				mTexture,
				wasInvalidated,
			};
		}

		auto Texture2DAsset::update() -> void
		{
			mInvalidated.store(true, std::memory_order_relaxed);
		}

		auto Texture2DAsset::load() -> std::shared_ptr<Graphics::Texture2D>
		{
			const auto rawData = std::visit([&](auto& image) { return image.load(); }, mImage);
			auto loadedTexture = Graphics::load2DTexture(rawData);
			auto texture	   = std::make_shared<Graphics::Texture2D>(nullptr);
			loadedTexture.swap(*texture);
			return texture;
		}

	} // namespace Assets
} // namespace HJUIK

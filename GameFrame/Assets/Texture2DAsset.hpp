#ifndef GAMEFRAME_ASSETS_TEXTURE2D_ASSET_HPP
#define GAMEFRAME_ASSETS_TEXTURE2D_ASSET_HPP

#include <atomic>
#include <variant>

#include "../Graphics/ImageLoader.hpp"
#include "../Graphics/Texture.hpp"
#include "Asset.hpp"
#include "FileAsset.hpp"

namespace HJUIK
{
	namespace Assets
	{
		namespace detail
		{
			struct ImageData {
				Graphics::Image2DLoader::RawData Data;

				auto load() -> Graphics::Image2DLoader::RawData;
				auto registerUpdateHandler(const std::function<void()>& callback) -> void;
			};

			struct ImageFile {
				std::shared_ptr<FileAsset> File;

				auto load() -> Graphics::Image2DLoader::RawData;
				auto registerUpdateHandler(std::function<void()> callback) -> void;
			};

			using Image = std::variant<ImageData, ImageFile>;
		} // namespace detail

		struct Texture2DAssetValue {
			// A shared_ptr to the current texture.
			// The user can reuse this texture as much as they want
			// (provided that they kept a reference to this), but they
			// would miss out on the newly hot-reloaded texture
			// Hence, it's advisable for one not to put this into any
			// long-term storage.
			std::shared_ptr<Graphics::Texture2D> Texture;
			// If this is true, the `Texture` has been recreated since
			// the last call to `Texture2DAsset::get()`
			// Also, `registerUpdateHandler` can also be used to achieve
			// somewhat similar functionalities.
			bool Invalidated;
		};

		class Texture2DAsset : public Asset
		{
		public:
			explicit Texture2DAsset(Graphics::Image2DLoader::RawData data);
			explicit Texture2DAsset(std::shared_ptr<FileAsset> file);
			explicit Texture2DAsset(Path path);
			~Texture2DAsset() override;
			Texture2DAsset(const Texture2DAsset&)					 = delete;
			Texture2DAsset(Texture2DAsset&&)						 = delete;
			auto operator=(const Texture2DAsset&) -> Texture2DAsset& = delete;
			auto operator=(Texture2DAsset&&) -> Texture2DAsset&		 = delete;

			auto get() -> Texture2DAssetValue;

		private:
			auto update() -> void;
			auto load() -> std::shared_ptr<Graphics::Texture2D>;

			detail::Image mImage;
			std::shared_ptr<Graphics::Texture2D> mTexture;
			std::atomic_bool mInvalidated{false};
		};
	} // namespace Assets

} // namespace HJUIK


#endif

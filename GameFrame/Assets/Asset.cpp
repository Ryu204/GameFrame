#include "Asset.hpp"

namespace HJUIK
{
	namespace Assets
	{
		UpdateHandle::UpdateHandle(std::weak_ptr<Asset> asset, std::size_t id) : mAsset(asset), mID(id) {}
		UpdateHandle::~UpdateHandle()
		{
			const auto asset = mAsset.lock();
			if (asset != nullptr) {
				asset->mUpdateHandlerIDGenerator.erase(mID);
				asset->mAssetUpdatedHandlers.erase(mID);
			}
		}

		auto Asset::registerUpdateHandler(std::function<void()> callback) -> UpdateHandle
		{
			const auto id = mUpdateHandlerIDGenerator.generate();
			mAssetUpdatedHandlers.emplace(id, std::move(callback));
			return UpdateHandle{std::weak_ptr<Asset>{shared_from_this()}, id};
		}

		auto Asset::assetUpdated() -> void
		{
			for (const auto& [id, updateHandler] : mAssetUpdatedHandlers) {
				updateHandler();
			}
		}
	} // namespace Assets
} // namespace HJUIK

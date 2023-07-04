#ifndef GAMEFRAME_ASSETS_ASSET_HPP
#define GAMEFRAME_ASSETS_ASSET_HPP

#include <functional>
#include <memory>
#include <vector>

#include "../Utilize/IDGenerator.hpp"

namespace HJUIK
{
	namespace Assets
	{
		class Asset;

		// returned from `registerUpdateHandler`
		// destroy this to unregister the handler
		class UpdateHandle
		{
		public:
			// default constructor will initialize this handle
			// to an invalid state (`mAsset` is nullptr, `id` is 0)
			UpdateHandle();
			UpdateHandle(std::weak_ptr<Asset> asset, std::size_t id);
			~UpdateHandle();

			UpdateHandle(UpdateHandle&&)						 = default;
			UpdateHandle(const UpdateHandle&)					 = delete;
			auto operator=(UpdateHandle&&) -> UpdateHandle&		 = default;
			auto operator=(const UpdateHandle&) -> UpdateHandle& = delete;

			auto isValid() const -> bool;

		private:
			std::weak_ptr<Asset> mAsset;
			std::size_t mID;
		};

		// Asset should always be stored in a shared_ptr in order to share
		// common assets throughout the program.
		class Asset : public std::enable_shared_from_this<Asset>
		{
		public:
			Asset()			 = default;
			virtual ~Asset() = default;

			// since this lives in a shared_ptr, there's no need for Asset
			// to be movable/copyable
			Asset(const Asset&)					   = delete;
			Asset(Asset&&)						   = delete;
			auto operator=(const Asset&) -> Asset& = delete;
			auto operator=(Asset&&) -> Asset&	   = delete;

			// register an update handler
			// NOTE: keep the return value to keep this register alive
			auto registerUpdateHandler(std::function<void()> callback) -> UpdateHandle;

		protected:
			// call this every time the asset update
			// e.g. file content change, asset dependencies updated, etc.
			// this will dispatch all active update handlers
			auto assetUpdated() -> void;

		private:
			static constexpr size_t MAX_UPDATE_HANDLERS = 100;
			std::unordered_map<std::size_t, std::function<void()>> mAssetUpdatedHandlers;
			Utilize::IDGenerator<MAX_UPDATE_HANDLERS> mUpdateHandlerIDGenerator;
			friend class UpdateHandle;
		};
	} // namespace Assets
} // namespace HJUIK


#endif

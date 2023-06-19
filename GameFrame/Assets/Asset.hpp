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
		class UpdateHandle
		{
		public:
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

		class Asset : public std::enable_shared_from_this<Asset>
		{
		public:
			Asset()			 = default;
			virtual ~Asset() = default;

			Asset(const Asset&)					   = delete;
			Asset(Asset&&)						   = delete;
			auto operator=(const Asset&) -> Asset& = delete;
			auto operator=(Asset&&) -> Asset&	   = delete;

			auto registerUpdateHandler(std::function<void()> callback) -> UpdateHandle;

		protected:
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

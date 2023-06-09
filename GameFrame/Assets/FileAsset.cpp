#include "FileAsset.hpp"

#include <utility>

namespace HJUIK
{
	namespace Assets
	{
		FileAsset::FileAsset(Path path, bool watchFile)
			: mPath(std::move(path)), mFileWatcher{[&]() -> std::optional<Utilize::FileWatch> {
				  if (watchFile) {
					  return std::make_optional<Utilize::FileWatch>(mPath, [&]() { this->assetUpdated(); });
				  }

				  return std::nullopt;
			  }()}
		{
		}

		auto FileAsset::getPath() const -> const Path&
		{
			return mPath;
		}
	} // namespace Assets
} // namespace HJUIK

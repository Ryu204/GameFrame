#ifndef GAMEFRAME_ASSETS_FILE_ASSET_HPP
#define GAMEFRAME_ASSETS_FILE_ASSET_HPP

#include <filesystem>
#include <fstream>
#include <optional>

#include "../Utilize/FileWatch.hpp"
#include "Asset.hpp"

namespace HJUIK
{
	namespace Assets
	{
		using Path = std::filesystem::path;

		class FileAsset : public Asset
		{
		public:
			explicit FileAsset(Path path, bool watchFile = true);
			~FileAsset() override = default;

			FileAsset(const FileAsset&) = delete;
			FileAsset(FileAsset&&)		= delete;

			auto operator=(const FileAsset&) -> FileAsset& = delete;
			auto operator=(FileAsset&&) -> FileAsset&	   = delete;

			auto getPath() const -> const Path&;

			template <typename CharT = char>
			auto open(bool binary = false) -> std::basic_ifstream<CharT>
			{
				return std::basic_ifstream<CharT>{
					mPath, binary ? std::ios_base::binary : static_cast<std::ios_base::openmode>(0)};
			}

			template <typename CharT = char>
			auto readBytes() -> std::vector<CharT>
			{
				auto stream = open(/*binary*/ true);
				return std::vector<CharT>{std::istreambuf_iterator<CharT>{stream}, std::istreambuf_iterator<CharT>{}};
			}

			template <typename CharT = char>
			auto readText() -> std::basic_string<CharT>
			{
				auto stream = open(/*binary*/ false);
				return std::basic_string<CharT>{
					std::istreambuf_iterator<CharT>{stream}, std::istreambuf_iterator<CharT>{}};
			}

		private:
			Path mPath;
			std::optional<Utilize::FileWatch> mFileWatcher;
		};
	} // namespace Assets

} // namespace HJUIK


#endif

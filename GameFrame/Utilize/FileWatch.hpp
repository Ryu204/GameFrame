#ifndef GAMEFRAME_UTILIZE_FILEWATCH_HPP
#define GAMEFRAME_UTILIZE_FILEWATCH_HPP

#include <cstddef>
#include <filesystem>
#include <functional>
#include <memory>
#include <utility>

namespace HJUIK
{
	namespace Utilize
	{
		using Path		= std::filesystem::path;
		using WatchFunc = std::function<void()>;

		class FileWatch
		{
		public:
			FileWatch(const Path& path, WatchFunc callback);

		private:
			class FileWatcherCallback : public efsw::FileWatchListener
			{
			public:
				FileWatcherCallback(const Path& path, WatchFunc callback);

				auto handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename,
					efsw::Action action, std::string oldFilename) -> void override;

			private:
				std::string mFilename;
				WatchFunc mCallback;
			};

			efsw::FileWatcher mWatcher;
			std::unique_ptr<FileWatcherCallback> mCallback;
		};
	} // namespace Utilize

} // namespace HJUIK

#endif

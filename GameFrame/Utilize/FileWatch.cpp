#include "FileWatch.hpp"

#include <map>
#include <vector>

#include "CallAssert.hpp"

namespace HJUIK
{
	namespace Utilize
	{
		FileWatch::FileWatch(const Path& path, WatchFunc callback)
			: mCallback{std::make_unique<FileWatcherCallback>(path, std::move(callback))}
		{
			HJUIK_ASSERT(path.has_parent_path(), "cannot watch file without parent path: ", path);
			mWatcher.addWatch(path.parent_path(), mCallback.get());
            mWatcher.watch();
		}

		FileWatch::FileWatcherCallback::FileWatcherCallback(const Path& path, WatchFunc callback)
			: mFilename(path.filename().string()), mCallback(std::move(callback))
		{
		}

		// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
		auto FileWatch::FileWatcherCallback::handleFileAction(efsw::WatchID /*watchid*/, const std::string& /*dir*/,
			const std::string& filename, efsw::Action /*action*/, std::string /*oldFilename*/) -> void
		{
			if (filename == mFilename) {
				mCallback();
			}
		}
	} // namespace Utilize
} // namespace HJUIK

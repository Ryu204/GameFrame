#ifndef GAMEFRAME_AUDIO_ILOADER_HPP
#define GAMEFRAME_AUDIO_ILOADER_HPP

/*
	Base class for sound loader
*/

#include <string>
#include <filesystem>

namespace HJUIK
{
	namespace Audio
	{
		class SoundBuffer;
		class ILoader
		{
		public:
			ILoader()								   = default;
			virtual ~ILoader()						   = default;
			ILoader(const ILoader&)					   = delete;
			ILoader(ILoader&&)						   = delete;
			auto operator=(const ILoader&) -> ILoader& = delete;
			auto operator=(ILoader&&) -> ILoader&	   = delete;

			// Open the files, load sound data (except for samples data) and move the current position to the first sample
			virtual auto open(std::filesystem::path& /*filepath*/) -> void = 0;
			// Load `numSamples` samples from current position and move cursor to the next unread sample
			// If numSamples exceeds remaining sample count, move cursor to EOL
			// Return: actual loaded samples count
			virtual auto load(std::size_t /*numSamples*/) -> std::size_t = 0;
			auto load(std::size_t sampleIndex, std::size_t numSamples) -> std::size_t
			{
				seek(sampleIndex);
				load(numSamples);
			}
			auto load(float second, std::size_t numSamples) -> std::size_t
			{
				seekSecond(second);
				load(numSamples);
			}
			// Copy the loaded data of latest `load` call into `target`
			// The loader needs not to be in a valid state for next `buffer` call until next `load` operation
			virtual auto buffer(SoundBuffer& /*target*/, std::size_t /*maxSamples*/) -> void = 0;

			virtual auto channels() -> std::size_t	= 0;
			virtual auto sampleRate() -> std::size_t = 0;

			// Derived classes must implement one of these two methods
			// Move the current position to the specified `sampleIndex` or `second`
			virtual auto seek(size_t sampleIndex) -> void = 0;
			auto seekSecond(float second) -> void
			{
				seek(static_cast<size_t>(channels() * sampleRate() * second));
			}
			virtual auto getCurrentSample() -> std::size_t = 0;
			virtual auto getCurrentTime() -> float
			{
				return { getCurrentSample() * 1.F / sampleRate() / channels() };
			}
		};
	} // namespace Audio
} // namespace HJUIK

#endif

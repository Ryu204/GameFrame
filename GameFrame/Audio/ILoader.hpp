#ifndef GAMEFRAME_AUDIO_ILOADER_HPP
#define GAMEFRAME_AUDIO_ILOADER_HPP

/*
	Base class for sound loader
*/

#include <iostream>
#include <string>

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

			// Load the sample data to the `target`, with `maxSamples` being the maximum
			// number of samples that could be loaded onto `target`.
            // Return the maximum number of samples (not samples per channel) loaded onto
            // the buffer, and if the returned value is less than `maxSamples`. However,
            // this does not mean that if this returned value is less than `maxSamples`
            // then EOF has been reached, since it could mean that there is not enough
            // space left on the buffer to put another sample (e.g. `maxSamples` is odd
            // but this loader have stereo format, i.e. 2 channels, pretty extreme case
            // but it could happen).
			virtual auto buffer(SoundBuffer& /*target*/, std::size_t /*maxSamples*/) -> std::size_t = 0;

			virtual auto channels() -> std::size_t	= 0;
			virtual auto sampleRate() -> std::size_t = 0;

			// implement one of these two methods
			virtual auto seek(std::size_t sampleIndex) -> void
			{
				seekSecond(static_cast<float>(sampleIndex) / static_cast<float>(sampleRate()));
			}

			virtual auto seekSecond(float second) -> void
			{
				seek(static_cast<std::size_t>(static_cast<float>(sampleRate()) * second));
			}
		};
	} // namespace Audio
} // namespace HJUIK

#endif

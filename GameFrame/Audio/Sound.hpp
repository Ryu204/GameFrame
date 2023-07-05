#ifndef GAMEFRAME_AUDIO_SOUND_HPP
#define GAMEFRAME_AUDIO_SOUND_HPP

#include <cstddef>
#include <vector>

#include "ILoader.hpp"
#include "SoundBuffer.hpp"
#include "SoundSource.hpp"

namespace HJUIK
{
	namespace Audio
	{
		// `Sound` is used to play short audio (which can be loaded into buffers)
		// `Sound` supports playing audio using multiple sources
		// (useful when the same sound effects can play concurrently)
		class Sound
		{
		public:
			Sound(std::size_t numSources, ILoader& loader);

            // return nullptr if all sources were playing concurrently
			auto getPlayableSource() -> SoundSource*;

		private:
			SoundBuffer mBuffer;
			std::vector<SoundSource> mSources;
		};
	} // namespace Audio

} // namespace HJUIK


#endif

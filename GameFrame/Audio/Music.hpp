#ifndef GAMEFRAME_AUDIO_MUSIC_HPP
#define GAMEFRAME_AUDIO_MUSIC_HPP

#include <atomic>
#include <cstddef>
#include <map>
#include <memory>
#include <utility>

#include "ILoader.hpp"
#include "SoundBuffer.hpp"
#include "SoundSource.hpp"

namespace HJUIK
{
	namespace Audio
	{
		// `Music` is used to play long audio (which will be streamed to buffers)
		// `Music` only supports playing audio using one source (streaming buffers
		// can't be shared among multiple `Music` sources)
		// Since periodically streaming music is typically done in another thread,
		// `Music` is thread-safe by design.
		class Music
		{
		public:
			static constexpr std::size_t NUM_BUFFERS		= 4;
			static constexpr std::size_t SAMPLES_PER_BUFFER = 65536;
			// NOTE: do not call setLoop in the `SoundSource` gotten from `getSource`
			explicit Music(std::shared_ptr<ILoader> loader, bool loop = false);

			auto getSource() -> SoundSource&;

			// returns false if there's nothing more to play (reach EOF with loop == false)
			auto update() -> bool;

			auto setLoop(bool loop) -> void;

			auto seek(std::size_t sampleIndex) -> void;

		private:
			std::shared_ptr<ILoader> mLoader;
			std::map<ALuint, SoundBuffer> mBuffers;
			SoundSource mSource;
			std::atomic_bool mLoop;
		};
	} // namespace Audio
} // namespace HJUIK
#endif

#ifndef GAMEFRAME_AUDIO_SOUND_STREAM_HPP
#define GAMEFRAME_AUDIO_SOUND_STREAM_HPP

/*
	Represent a streamed source (usually containing a large amount of data played by portions)
*/

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

#include "OpenALWrapper.hpp"
#include "SoundSource.hpp"
#include "Utilize.hpp"

namespace HJUIK
{
	namespace Audio
	{
		// the streaming mechanism is somewhat similar to
		// https://github.com/btmxh/rhythm-game-boilerplate/blob/master/src/main/java/com/dah/rgb/audio/Music.java
		class SoundStream : public SoundSource
		{
		public:
			static constexpr std::size_t NUM_BUFFERS		= 4;
			static constexpr std::size_t SAMPLES_PER_BUFFER = 65536;

			SoundStream(std::shared_ptr<ILoader> loader, bool loop = false) : mLoader{std::move(loader)}, mLoop{loop}
			{
				for (std::size_t i = 0; i < NUM_BUFFERS; ++i) {
					mBuffers[i] = std::make_unique<SoundBuffer>();
					// try to put data on the buffer
					// if nothing was loaded, EOF was reached
					if (stream(*mBuffers[i]) == 0) {
						break;
					}

					auto buffer = mBuffers[i]->get();
					// queue the buffer
					alCheck(alSourceQueueBuffers(get(), 1, &buffer));
				}
			}

			// this needs to be periodically called (in an audio thread or something)
			auto update() -> void
			{
				const auto numProcessed = alGet<ALint>(alGetSourcei, get(), AL_BUFFERS_PROCESSED);
				HJUIK_ASSERT(numProcessed >= 0, "num of processed buffers should be non-negative");
				for (std::size_t i = 0; i < numProcessed; ++i) {
					ALuint bufferHandle;
					alCheck(alSourceUnqueueBuffers(get(), 1, &bufferHandle));
					SoundBuffer buffer{bufferHandle};
					buffer.reset();
				}
			}

		private:
			std::shared_ptr<ILoader> mLoader;
			std::atomic_bool mLoop;
			std::recursive_mutex mMutex;
			std::array<std::unique_ptr<SoundBuffer>, NUM_BUFFERS> mBuffers;

			auto stream(SoundBuffer& buffer) -> std::size_t
			{
				std::size_t numSamples = 0;

				while (numSamples < SAMPLES_PER_BUFFER) {
				}
			}
		};
	} // namespace Audio
} // namespace HJUIK

#endif

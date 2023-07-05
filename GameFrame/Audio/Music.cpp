#include "Music.hpp"


namespace HJUIK
{
	namespace Audio
	{
		Music::Music(std::shared_ptr<ILoader> loader, bool loop) : mLoader{std::move(loader)}, mLoop{loop}
		{
			for (std::size_t i = 0; i < NUM_BUFFERS; ++i) {
				SoundBuffer buffer{};
				if (mLoader->buffer(buffer, SAMPLES_PER_BUFFER) == 0) {
					break;
				}

				mSource.queueBuffer(buffer);
				mBuffers.emplace(buffer.get(), std::move(buffer));
			}
		}

		auto Music::getSource() -> SoundSource&
		{
			return mSource;
		}

		auto Music::update() -> bool
		{
			const auto numProcessed =
				static_cast<std::size_t>(alGet<ALint>(alGetSourcei, mSource.get(), AL_BUFFERS_PROCESSED));
			for (std::size_t i = 0; i < numProcessed; ++i) {
				const auto bufferHandle = mSource.unqueueBuffer();
				auto& buffer			= mBuffers.at(bufferHandle);

				if (mLoader->buffer(buffer, SAMPLES_PER_BUFFER) == 0) {
					bool exit = true;
					if (mLoop.load(std::memory_order_relaxed)) {
						mLoader->seek(0);
						exit = mLoader->buffer(buffer, SAMPLES_PER_BUFFER) == 0;
					}

					if (exit) {
						return false;
					}
				}

				mSource.queueBuffer(buffer);
			}

			if (numProcessed == NUM_BUFFERS) {
				// this is not updated frequent enough
				mSource.play();
			}

			return true;
		}

		auto Music::setLoop(bool loop) -> void
		{
			mLoop.store(loop, std::memory_order_relaxed);
		}

		auto Music::seek(std::size_t sampleIndex) -> void
		{
			mLoader->seek(sampleIndex);
		}
	} // namespace Audio
} // namespace HJUIK

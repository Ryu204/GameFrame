#ifndef GAMEFRAME_AUDIO_SOUND_STREAM_HPP
#define GAMEFRAME_AUDIO_SOUND_STREAM_HPP

/*
    Represent a streamed source (usually containing a large amount of data played by portions)
*/

#include <cstdint>
#include <array>
#include <memory>

#include "Utilize.hpp"
#include "OpenALWrapper.hpp"
#include "SoundSource.hpp"

namespace HJUIK
{
    namespace Audio
    {
        class SoundStream
        {
        public:
            static constexpr std::size_t NUM_BUFFERS = 4;
            static constexpr std::size_t SAMPLES_PER_BUFFER = 65536;
            static constexpr std::size_t CHECK_DELAY = 10; // milliseconds

            SoundStream(std::filesystem::path filepath) : mPath{std::move(filepath)} 
            {
                // Use pointer to allocate buffers on the heap
                for (int i = 0; i < NUM_BUFFERS; ++i)
                {
                    mBuffers[i] = std::make_unique<SoundBuffer>();
                }
            };

        private:
            void startThread()
            {
                mFileStreamer->open(mPath);

                std::size_t topQueueBuffer = 0;
                bool reachedEOF = false;

                // Initialize first NUM_BUFFERS buffers
                for (int i = 0; i < NUM_BUFFERS; ++i)
                {
                    auto loadCount = mFileStreamer->load(SAMPLES_PER_BUFFER);
                    mFileStreamer->buffer(*mBuffers[i], loadCount);
                    mSource.queueBuffer(*mBuffers[i]);
                    // If this is the last chunk in the file
                    if (loadCount < SAMPLES_PER_BUFFER)
                    {
                        reachedEOF = true;
                        break;
                    }
                }

                // Start playing
                mSource.play();

                while (true)
                {
                    if (reachedEOF)
                    {
                        break;
                    }

                    auto processedBuffer = mSource.getProcessedBufferCount();
                    // Add data to the available buffers
                    for (int i = 0; i < processedBuffer; ++i)
                    {
                        auto loadCount = mFileStreamer->load(SAMPLES_PER_BUFFER);
                        // If this is the last chunk in the file
                        if (loadCount < SAMPLES_PER_BUFFER)
                        {
                            reachedEOF = true;
                        }
                        // unqueue the earliest buffer
                        mSource.unqueueBuffer();
                        mFileStreamer->buffer(*mBuffers[topQueueBuffer], SAMPLES_PER_BUFFER);
                        mSource.queueBuffer(*mBuffers[topQueueBuffer]);
                        topQueueBuffer = (topQueueBuffer + 1) % NUM_BUFFERS;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(CHECK_DELAY));
                }

                while (mSource.getState() != SourceState::STOPPED)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(CHECK_DELAY));
                }

                // Cleanup
                // ...
            }

        private:
            SoundSource mSource;
            std::array<std::unique_ptr<SoundBuffer>, NUM_BUFFERS> mBuffers;

            std::filesystem::path mPath;
            std::unique_ptr<ILoader> mFileStreamer;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
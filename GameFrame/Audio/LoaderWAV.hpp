#ifndef GAMEFRAME_AUDIO_LOADER_WAV_HPP
#define GAMEFRAME_AUDIO_LOADER_WAV_HPP

#include <cstdint>
#include <exception>
#include "deps/AudioFile.hpp"
#include "ILoader.hpp"
#include "SoundBuffer.hpp"
#include "../Utilize/CallAssert.hpp"

namespace HJUIK
{
    namespace Audio
    {   
        class LoaderWAV : public ILoader
        {
        public:
            // If the load fails, throw an exception
            auto loadFromFile(const char* filename) -> void
            {
                mInternalLoader.shouldLogErrorsToConsole(/*logErrors=*/false);
                if (!mInternalLoader.load(std::string(filename)))
                {
                    throw std::runtime_error("Cannot load WAV file in " + std::string(filename));
                }
            }

            auto buffer(SoundBuffer& target) -> bool override
            {
                auto& info = target.getData();
                // Format & Data ==================================================
                int bitdepth = mInternalLoader.getBitDepth();
                info.SamplePerSecond = mInternalLoader.getSampleRate();
                // Make sure openAL can handle file format ========================
                // NOLINTBEGIN(*-magic-numbers)
                if (bitdepth != 8 && bitdepth != 16)
                {
                    throw std::runtime_error("WAV file bit depth is not 8 or 16");
                }
                int channelNum = mInternalLoader.getNumChannels();
                if (channelNum != 1 && channelNum != 2)
                {
                    throw std::runtime_error("WAV file channel count is not 1 or 2");
                }
                // ================================================================
                if (mInternalLoader.isMono())
                {
                    if (bitdepth == 8)
                    {
                        info.Format = BufferFormat::MONO8;
                        mono8DataBuffer(target);
                    }
                    else
                    {
                        info.Format = BufferFormat::MONO16;
                        mono16DataBuffer(target);
                    }
                }
                else
                {
                    if (bitdepth == 8)
                    {
                        info.Format = BufferFormat::STEREO8;
                        stereo8DataBuffer(target);
                    }
                    else
                    {
                        info.Format = BufferFormat::STEREO16;
                        stereo16DataBuffer(target);
                    }
                }
                // NOLINTEND(*-magic-numbers)
                // free memory
                mInternalLoader.samples.clear();
                return true;
            }

        private:
            auto mono8DataBuffer(SoundBuffer& target) const -> void
            {
                auto& targetVector = target.getData().V8Bit;
                targetVector.clear();
                const auto& data = mInternalLoader.samples;
                HJUIK_ASSERT(data.size() == 1, "Invalid call");
                targetVector.reserve(data[0].size());
                for (const auto& sample : data[0])
                {
                    // We need a mapping from [0, UINT16_MAX] -> [0, UINT8_MAX]
                    targetVector.push_back(static_cast<std::uint8_t>(
                        sample * 1.0 * UINT8_MAX / UINT16_MAX
                    ));
                }
                target.getData().V16Bit.clear();
            }

            auto mono16DataBuffer(SoundBuffer& target) -> void
            {
                auto& data = mInternalLoader.samples;
                HJUIK_ASSERT(data.size() == 1, "Invalid call");
                target.getData().V16Bit.swap(data[0]);
                target.getData().V8Bit.clear();
            }

            auto stereo8DataBuffer(SoundBuffer& target) const -> void
            {
                auto& targetVector = target.getData().V8Bit;
                targetVector.clear();

                const auto& data = mInternalLoader.samples;
                HJUIK_ASSERT(data.size() == 2, "Invalid call");
                targetVector.reserve(data[0].size() * 2);

                for (int i = 0; i < data[0].size(); ++i)
                {
                    // We need a mapping from [0, UINT16_MAX] -> [0, UINT8_MAX]
                    // OpenAL specs says the data is interleaved (L,R,L,R,...)
                    targetVector.push_back(static_cast<std::uint8_t>(
                        data[0][i] * 1.0 * UINT8_MAX / UINT16_MAX
                    ));
                    targetVector.push_back(static_cast<std::uint8_t>(
                        data[1][i] * 1.0 * UINT8_MAX / UINT16_MAX
                    ));
                }
                target.getData().V16Bit.clear();
            }

            auto stereo16DataBuffer(SoundBuffer& target) const -> void
            {
                auto& targetVector = target.getData().V16Bit;
                targetVector.clear();

                const auto& data = mInternalLoader.samples;
                HJUIK_ASSERT(data.size() == 2, "Invalid call");

                for (int i = 0; i < data[0].size(); ++i)
                {
                    // We need a mapping from [0, UINT16_MAX] -> [0, UINT16_MAX]
                    // OpenAL specs says the data is interleaved (L,R,L,R,...)
                    targetVector.push_back(data[0][i]);
                    targetVector.push_back(data[1][i]);
                }
                target.getData().V8Bit.clear();
            }
            
            AudioFile<std::uint16_t> mInternalLoader;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
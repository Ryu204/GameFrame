#ifndef GAMEFRAME_AUDIO_SOUND_BUFFER_HPP
#define GAMEFRAME_AUDIO_SOUND_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <AL/al.h>
#include "ILoader.hpp"

namespace HJUIK
{
    namespace Audio
    {
        enum class BufferState : decltype(AL_UNUSED)
        {
            UNUSED = AL_UNUSED,
            PROCESSED = AL_PROCESSED,
            PENDING = AL_PENDING

        };

        enum class BufferFormat : decltype(AL_FORMAT_MONO8)
        {
            // STEREO* format does not support spatialization

            MONO8 = AL_FORMAT_MONO8,
            MONO16 = AL_FORMAT_MONO16,
            STEREO8 = AL_FORMAT_STEREO8,
            STEREO16 = AL_FORMAT_STEREO16,
        };

        class SoundBuffer
        {
        public:
            // Numerics information of the buffer
            struct Info
            {
                BufferState State{BufferState::UNUSED};
                BufferFormat Format{BufferFormat::MONO16};
            };
            // Only the specified format will contain meaningful data
            // The other vector is left empty
            struct Data
            {
                std::vector<std::uint8_t> V8Bit;
                std::vector<std::uint16_t> V16Bit;
            };

            SoundBuffer();
            ~SoundBuffer();

            SoundBuffer(const SoundBuffer&) = delete;
            SoundBuffer(SoundBuffer&&) = delete;
            auto operator = (SoundBuffer&&) -> SoundBuffer& = delete;
            auto operator = (const SoundBuffer&) -> SoundBuffer& = delete;

            // Get numerics information of the buffer
            auto getInfo() const -> Info;
            // Get the actual sound data (internal use only)
            auto getRawData() const -> const void*;
            // Get the vector sound data (internal use only)
            auto getVectorData() -> Data&;
            // Buffer the data from a Loader
            auto bufferData(ILoader& loader) -> void;
        private:
            unsigned int mID {0};
            Info mInfo;
            Data mData;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
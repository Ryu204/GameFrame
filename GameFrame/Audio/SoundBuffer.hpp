#ifndef GAMEFRAME_AUDIO_SOUND_BUFFER_HPP
#define GAMEFRAME_AUDIO_SOUND_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <AL/al.h>
#include "ILoader.hpp"
#include "Error.hpp"
#include "OpenALWrapper.hpp"

namespace HJUIK
{
    namespace Audio
    {
        namespace detail
        {
            struct BufferTrait
            {
                using HandleType = ALuint;
                static auto create() -> HandleType
                {
                    HandleType handle = 0;
                    alCheck(alGenBuffers(1, &handle));
                    return handle;
                }
                static auto destroy(HandleType handle) -> void
                {
                    alCheck(alDeleteBuffers(1, &handle));
                }
            };
        } // namespace detail

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

        class SoundBuffer : public OpenALWrapper<detail::BufferTrait>
        {
        public:
            // Only the specified format will contain meaningful data
            // The other vector is left empty
            struct Data
            {
                std::vector<std::uint8_t> V8Bit{};
                std::vector<std::uint16_t> V16Bit{};

                BufferState State{BufferState::UNUSED};
                BufferFormat Format{BufferFormat::MONO16};
                std::size_t SamplePerSecond{0};
            };

            // Get the vector sound data (internal use only)
            auto getData() -> Data&;
            // Buffer the data from a Loader
            auto bufferData(ILoader& loader) -> void;
        private:
            auto is8Bit() const -> bool;

            Data mData;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
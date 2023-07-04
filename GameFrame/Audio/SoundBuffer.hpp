#ifndef GAMEFRAME_AUDIO_SOUND_BUFFER_HPP
#define GAMEFRAME_AUDIO_SOUND_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <AL/al.h>
#include "ILoader.hpp"
#include "Utilize.hpp"
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
                    return alGen<HandleType>(alGenBuffers);
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
            // Fetch the data to actual OpenAL's implementation
            // This function should be used by ILoader
            auto uploadData(const void* data, BufferFormat format, int sizeInBytes, int sampleRate) -> void;
            auto isValid() const -> bool;
            // These functions are meaningful only when `isValid()==true`
            auto is8Bit() const -> bool;
            auto isMono() const -> bool;
        
        private:
            BufferFormat mFormat{BufferFormat::STEREO16};
            bool mIsValid{false};
        };
    } // namespace Audio
} // namespace HJUIK

#endif
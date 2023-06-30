#ifndef GAMEFRAME_AUDIO_SOUND_SOURCE_HPP
#define GAMEFRAME_AUDIO_SOUND_SOURCE_HPP

#include "Error.hpp"
#include "OpenALWrapper.hpp"

namespace HJUIK
{
    namespace Audio
    {
        namespace detail
        {
            struct SourceTrait 
            {
                using HandleType = ALuint;
                static auto create() -> HandleType
                {
                    HandleType handle = 0;
                    alCheck(alGenSources(1, &handle));
                    return handle;
                }

                static auto destroy(HandleType handle) -> void
                {
                    alCheck(alDeleteSources(1, &handle));
                }
            };
        } // namespace detail

        enum class SourceState : decltype(AL_PLAYING)
        {
            PLAYING = AL_PLAYING,

        };

        class SoundSource : OpenALWrapper<detail::SourceTrait>
        {
        public:


        private:
            
        };
    } // namespace Audio
} // namespace HJUIK

#endif
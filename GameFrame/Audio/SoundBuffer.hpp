#ifndef GAMEFRAME_AUDIO_SOUND_BUFFER_HPP
#define GAMEFRAME_AUDIO_SOUND_BUFFER_HPP

#include <AL/al.h>
#include "Error.hpp"

namespace HJUIK
{
    namespace Audio
    {
        class SoundBuffer
        {
        public:
            SoundBuffer()
            {
                alCheck(alGenBuffers(1, &mID));
            }

            ~SoundBuffer()
            {
                alCheck(alDeleteBuffers(1, &mID));
            }

        private:
            unsigned int mID;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
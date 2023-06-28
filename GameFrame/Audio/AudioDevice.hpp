#ifndef GAMEFRAME_AUDIO_AUDIO_DEVICE_HPP
#define GAMEFRAME_AUDIO_AUDIO_DEVICE_HPP

/*
    The whole application uses at most 1 ALCdevice
*/

#include <memory>
#include <AL/al.h>
#include <AL/alc.h>
#include "Error.hpp"

namespace HJUIK
{
    namespace Audio
    {
        class AudioDevice
        {
        public:
            AudioDevice()
            {
                alCheck(mDevice = alcOpenDevice(nullptr));
                alCheck(mContext = alcCreateContext(mDevice, nullptr));
                alCheck(alcMakeContextCurrent(mContext));
            }

            ~AudioDevice()
            {
                if (!mContext)
                {
                    alCheck(alcMakeContextCurrent(nullptr));
                    alCheck(alcDestroyContext(mContext));
                }
                if (!mDevice)
                {
                    alCheck(alcCloseDevice(mDevice));
                }
            }

            auto getPtr() -> ALCdevice*
            {
                return mDevice;
            }

        private:
            ALCdevice* mDevice;
            ALCcontext* mContext;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
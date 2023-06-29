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
            AudioDevice(AudioDevice&&) = delete;
            AudioDevice(const AudioDevice&) = delete;
            auto operator = (const AudioDevice&) -> AudioDevice& = delete;
            auto operator = (AudioDevice&&) -> AudioDevice& = delete;
            AudioDevice()
                : mDevice(alcOpenDevice(nullptr))
            {
                if (mDevice == nullptr)
                {
                    throw std::runtime_error("Cannot open an OpenAL device");
                }
                alcCheck(mContext = alcCreateContext(mDevice, nullptr), mDevice);
                alcCheck(alcMakeContextCurrent(mContext), mDevice);
            }

            ~AudioDevice()
            {
                if (mContext != nullptr)
                {
                    alcCheck(alcMakeContextCurrent(nullptr), mDevice);
                    alcCheck(alcDestroyContext(mContext), mDevice);
                }
                if (mDevice != nullptr)
                {
                    alcCheck(alcCloseDevice(mDevice), mDevice);
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
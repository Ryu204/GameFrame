#ifndef GAMEFRAME_AUDIO_ILOADER_HPP
#define GAMEFRAME_AUDIO_ILOADER_HPP

/*
    Base class for sound loader
*/

#include <string>

namespace HJUIK
{
    namespace Audio
    {   
        class SoundBuffer;
        class ILoader
        {
        public:
            ILoader() = default;
            virtual ~ILoader() = default;
            ILoader(const ILoader&) = delete;
            ILoader(ILoader&&) = delete;
            auto operator = (const ILoader&) -> ILoader& = delete;
            auto operator = (ILoader&&) -> ILoader& = delete;

            // Copy the loaded data into `target`
            // Do not actually buffer data to OpenAL (this is done inside SoundBuffer)
            // The loader needs not to be in a valid state for next `buffer` call until next `load` operation
            virtual auto buffer(SoundBuffer& target) -> void = 0;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
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

            // Buffer the data loaded into `target`
            // No need to be available until next load
            virtual auto buffer(SoundBuffer& target) -> void = 0;
        };
    } // namespace Audio
} // namespace HJUIK

#endif
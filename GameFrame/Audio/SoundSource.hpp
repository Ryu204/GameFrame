#ifndef GAMEFRAME_AUDIO_SOUND_SOURCE_HPP
#define GAMEFRAME_AUDIO_SOUND_SOURCE_HPP

#include "Utilize.hpp"
#include "OpenALWrapper.hpp"
#include "SoundBuffer.hpp"

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
                    return alGen<HandleType>(alGenSources);
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
            PAUSED = AL_PAUSED,
            STOPPED = AL_STOPPED,
            INITIAL = AL_INITIAL
        };

        class SoundSource : OpenALWrapper<detail::SourceTrait>
        {
        public:
            // Determine whether space information of source is relative to listener
            auto setRelative(bool relativity = true) -> void
            {
                alCheck(alSourcei(get(), AL_SOURCE_RELATIVE, relativity));
            };
            auto getRelative() const -> bool
            {
                return alGet<ALint>(alGetSourcei, get(), AL_SOURCE_RELATIVE) == AL_TRUE;
            }
            auto setLoop(bool loop = true) -> void
            {
                alCheck(alSourcei(get(), AL_LOOPING, loop));
            };
            auto getLoop() const -> bool
            {
                return alGet<ALint>(alGetSourcei, get(), AL_LOOPING) == AL_TRUE;
            }
            auto setBuffer(const SoundBuffer& buffer) -> void
            {
                checkBuffer(buffer);
                alCheck(alSourcei(get(), AL_BUFFER, buffer.get()));
            }
            auto queueBuffer(const SoundBuffer& buffer) -> void
            {
                checkBuffer(buffer);
                const auto bufferName = buffer.get();
                alSourceQueueBuffers(get(), 1, &bufferName); 
            }
            auto unqueueBuffer(const SoundBuffer& buffer) -> void
            {
                // No need to check buffers, their data is already copied
                auto bufferName = buffer.get();
                alSourceUnqueueBuffers(get(), 1, &bufferName); 
            }
            auto getState() const -> SourceState
            {
                return static_cast<SourceState>(alGet<ALint>(alGetSourcei, get(), AL_SOURCE_STATE));
            }

            auto play() -> void
            {
                alCheck(alSourcePlay(get()));
            }
            auto pause() -> void
            {
                alCheck(alSourcePause(get()));
            }
            auto stop() -> void
            {
                alCheck(alSourceStop(get()));
            }
            auto rewind() -> void
            {
                alCheck(alSourceRewind(get()));
            }
        private:
            static auto checkBuffer(const SoundBuffer& buffer) -> void
            {
                if (!buffer.isValid())
                {
                    throw std::runtime_error("Audio: An invalid buffer was set to a source");
                }
            }
        };
    } // namespace Audio
} // namespace HJUIK

#endif
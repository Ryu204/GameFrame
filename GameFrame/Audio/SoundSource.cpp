#include "SoundSource.hpp"
#include <cmath>
#include "Utilize.hpp"
#include "OpenALWrapper.hpp"
#include "SoundBuffer.hpp"
#include "../Utilize/GLMTypedef.hpp"

namespace HJUIK
{
    namespace Audio
    {
        auto SoundSource::setRelative(bool relativity = true) -> void
        {
            alCheck(alSourcei(get(), AL_SOURCE_RELATIVE, relativity));
        }
        
        auto SoundSource::getRelative() const -> bool
        {
            return alGet<ALint>(alGetSourcei, get(), AL_SOURCE_RELATIVE) == AL_TRUE;
        }

        auto SoundSource::setLoop(bool loop = true) -> void
        {
            alCheck(alSourcei(get(), AL_LOOPING, loop));
        }

        auto SoundSource::getLoop() const -> bool
        {
            return alGet<ALint>(alGetSourcei, get(), AL_LOOPING) == AL_TRUE;
        }

        auto SoundSource::setBuffer(const SoundBuffer& buffer) -> void
        {
            checkBuffer(buffer);
            alCheck(alSourcei(get(), AL_BUFFER, buffer.get()));
        }

        auto SoundSource::queueBuffer(const SoundBuffer& buffer) -> void
        {
            checkBuffer(buffer);
            const auto bufferName = buffer.get();
            alSourceQueueBuffers(get(), 1, &bufferName); 
        }

        auto SoundSource::unqueueBuffer(const SoundBuffer& buffer) -> void
        {
            // No need to check buffers, their data is already copied
            auto bufferName = buffer.get();
            alSourceUnqueueBuffers(get(), 1, &bufferName); 
        }

        // This will not change the source position (:'>)
        auto SoundSource::setVelocity(const Vector3f velocity) -> void
        {
            alCheck(alSource3f(get(), AL_VELOCITY, velocity.x, velocity.y, velocity.z));
        }

        auto SoundSource::getVelocity() const -> Vector3f
        {
            Vector3f velocity{};
            alCheck(alGetSource3f(get(), AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z));
            return velocity;
        }

        // This will not change the source velocity
        auto SoundSource::setPosition (const Vector3f position) -> void
        {
            alCheck(alSource3f(get(), AL_VELOCITY, position.x, position.y, position.z));
        }

        auto SoundSource::getPosition () const -> Vector3f
        {
            Vector3f position{};
            alCheck(alGetSource3f(get(), AL_VELOCITY, &position.x, &position.y, &position.z));
            return position;
        }

        auto SoundSource::setPitch(float pitch) -> void
        {
            pitch = std::abs(pitch);
            if (pitch == 0.F)
            {
                // NOLINTBEGIN(*-magic-numbers)
                pitch = 1e-5;
                // NOLINTEND(*-magic-numbers)
            }
            alCheck(alSourcef(get(), AL_PITCH, pitch));
        }

        auto SoundSource::getPitch() const -> float
        {
            return alGet<ALfloat>(alGetSourcef, get(), AL_PITCH);
        }

        auto SoundSource::getState() const -> SourceState
        {
            return static_cast<SourceState>(alGet<ALint>(alGetSourcei, get(), AL_SOURCE_STATE));
        }

        auto SoundSource::play() -> void
        {
            alCheck(alSourcePlay(get()));
        }

        auto SoundSource::pause() -> void
        {
            alCheck(alSourcePause(get()));
        }

        auto SoundSource::stop() -> void
        {
            alCheck(alSourceStop(get()));
        }

        auto SoundSource::rewind() -> void
        {
            alCheck(alSourceRewind(get()));
        }

        auto SoundSource::checkBuffer(const SoundBuffer& buffer) -> void
        {
            if (!buffer.isValid())
            {
                throw std::runtime_error("Audio: An invalid buffer was set to a source");
            }
        }
    } // namespace Audio
} // namespace HJUIK
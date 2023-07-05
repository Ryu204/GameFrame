#include "SoundSource.hpp"

#include <cmath>

#include "../Utilize/GLMTypedef.hpp"
#include "OpenALWrapper.hpp"
#include "SoundBuffer.hpp"
#include "Utilize.hpp"

namespace HJUIK
{
	namespace Audio
	{
		auto SoundSource::setRelative(bool relativity) -> void
		{
			alCheck(alSourcei(get(), AL_SOURCE_RELATIVE, relativity));
		}

		auto SoundSource::getRelative() const -> bool
		{
			return alGet<ALint>(alGetSourcei, get(), AL_SOURCE_RELATIVE) == AL_TRUE;
		}

		auto SoundSource::setLoop(bool loop) -> void
		{
			alCheck(alSourcei(get(), AL_LOOPING, loop));
		}

		auto SoundSource::getLoop() const -> bool
		{
			return alGet<ALint>(alGetSourcei, get(), AL_LOOPING) == AL_TRUE;
		}

		auto SoundSource::setBuffer(const SoundBuffer& buffer) -> void
		{
			alCheck(alSourcei(get(), AL_BUFFER, buffer.get()));
		}

		auto SoundSource::queueBuffer(const SoundBuffer& buffer) -> void
		{
			const auto bufferName = buffer.get();
			alSourceQueueBuffers(get(), 1, &bufferName);
		}

		auto SoundSource::unqueueBuffer() -> ALuint
		{
            ALuint handle = 0;
			alSourceUnqueueBuffers(get(), 1, &handle);
            return handle;
		}

		// This will not change the source position (:'>)
		auto SoundSource::setVelocity(Vector3f velocity) -> void
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
		auto SoundSource::setPosition(Vector3f position) -> void
		{
			alCheck(alSource3f(get(), AL_POSITION, position.x, position.y, position.z));
		}

		auto SoundSource::getPosition() const -> Vector3f
		{
			Vector3f position{};
			alCheck(alGetSource3f(get(), AL_POSITION, &position.x, &position.y, &position.z));
			return position;
		}

		auto SoundSource::setPitch(float pitch) -> void
		{
			pitch = std::abs(pitch);
			if (pitch == 0.F) {
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

		auto SoundSource::setGain(float min, float max) -> void
		{
			alCheck(alSourcef(get(), AL_MAX_GAIN, max));
			alCheck(alSourcef(get(), AL_MIN_GAIN, min));
		}

		auto SoundSource::getMinGain() const -> float
		{
			return alGet<ALfloat>(alGetSourcef, get(), AL_MIN_GAIN);
		}

		auto SoundSource::getMaxGain() const -> float
		{
			return alGet<ALfloat>(alGetSourcef, get(), AL_MAX_GAIN);
		}

		auto SoundSource::setReferenceDistance(float distance) -> void
		{
			alCheck(alSourcef(get(), AL_REFERENCE_DISTANCE, distance));
		}

		auto SoundSource::getReferenceDistance() const -> float
		{
			return alGet<ALfloat>(alGetSourcef, get(), AL_REFERENCE_DISTANCE);
		}

		auto SoundSource::setRollOffFactor(float factor) -> void
		{
			alCheck(alSourcef(get(), AL_ROLLOFF_FACTOR, factor));
		}

		auto SoundSource::getRollOffFactor() const -> float
		{
			return alGet<ALfloat>(alGetSourcef, get(), AL_ROLLOFF_FACTOR);
		}

		auto SoundSource::setMaxDistance(float distance) -> void
		{
			alCheck(alSourcef(get(), AL_MAX_DISTANCE, distance));
		}

		auto SoundSource::getMaxDistance() const -> float
		{
			return alGet<ALfloat>(alGetSourcef, get(), AL_MAX_DISTANCE);
		}

		auto SoundSource::setDirection(Vector3f direction) -> void
		{
			alCheck(alSource3f(get(), AL_DIRECTION, direction.x, direction.y, direction.z));
		}

		auto SoundSource::getDirection() const -> Vector3f
		{
			Vector3f direction{};
			alCheck(alGetSource3f(get(), AL_POSITION, &direction.x, &direction.y, &direction.z));
			return direction;
		}

		auto SoundSource::setInnerAngle(float angle) -> void
		{
			alCheck(alSourcef(get(), AL_CONE_INNER_ANGLE, angle));
		}

		auto SoundSource::getInnerAngle() const -> float
		{
			return alGet<float>(alGetSourcef, get(), AL_CONE_INNER_ANGLE);
		}

		auto SoundSource::setOuterAngle(float angle) -> void
		{
			alCheck(alSourcef(get(), AL_CONE_OUTER_ANGLE, angle));
		}

		auto SoundSource::getOuterAngle() const -> float
		{
			return alGet<float>(alGetSourcef, get(), AL_CONE_OUTER_ANGLE);
		}

		auto SoundSource::setOuterGain(float gain) -> void
		{
			alCheck(alSourcef(get(), AL_CONE_OUTER_GAIN, gain));
		}

		auto SoundSource::getOuterGain() const -> float
		{
			return alGet<float>(alGetSourcef, get(), AL_CONE_OUTER_GAIN);
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
	} // namespace Audio
} // namespace HJUIK

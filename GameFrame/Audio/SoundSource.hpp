#ifndef GAMEFRAME_AUDIO_SOUND_SOURCE_HPP
#define GAMEFRAME_AUDIO_SOUND_SOURCE_HPP

/*
	Represent a point/object emmiting sounds in a 3D space
*/

#include <cmath>

#include "../Utilize/GLMTypedef.hpp"
#include "OpenALWrapper.hpp"
#include "SoundBuffer.hpp"
#include "Utilize.hpp"

namespace HJUIK
{
	namespace Audio
	{
		namespace detail
		{
			struct SourceTrait {
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

		enum class SourceState : decltype(AL_PLAYING) {
			PLAYING = AL_PLAYING,
			PAUSED	= AL_PAUSED,
			STOPPED = AL_STOPPED,
			INITIAL = AL_INITIAL
		};

		class SoundSource : public OpenALWrapper<detail::SourceTrait>
		{
		public:
			using OpenALWrapper::OpenALWrapper;
			// Determine whether space information of source is relative to listener
			auto setRelative(bool relativity = true) -> void;
			auto getRelative() const -> bool;
			// Set if the source is looped
			auto setLoop(bool loop = true) -> void;
			auto getLoop() const -> bool;
			// Set the underlying buffer
			// Latest set buffer will be play first when call `play()`
			auto setBuffer(const SoundBuffer& buffer) -> void;
			// Queue buffer to be played after current buffer on source
			auto queueBuffer(const SoundBuffer& buffer) -> void;
			// Unqueue buffer (get handle to a buffer that was previously queued but was done playing)
			auto unqueueBuffer() -> ALuint;
			// This will not change the source position
			// Used for stuffs like Doppler effect
			auto setVelocity(Vector3f velocity) -> void;
			auto getVelocity() const -> Vector3f;
			// This will not change the source velocity
			// Used for spatialization
			auto setPosition(Vector3f position) -> void;
			auto getPosition() const -> Vector3f;
			// identity = 1.F
			// double = +1 octave
			// half = -1 octave
			auto setPitch(float pitch) -> void;
			auto getPitch() const -> float;
			// Set limit of source volume
			auto setGain(float min = 0.F, float max = 1.F) -> void;
			auto getMinGain() const -> float;
			auto getMaxGain() const -> float;
			// Set spatialization properties
			auto setReferenceDistance(float distance) -> void;
			auto getReferenceDistance() const -> float;
			auto setRollOffFactor(float factor) -> void;
			auto getRollOffFactor() const -> float; // Pass 0 to disable attenuation
			auto setMaxDistance(float distance) -> void;
			auto getMaxDistance() const -> float;
			// Directional sound - cone
			auto setDirection(Vector3f direction) -> void;
			auto getDirection() const -> Vector3f;
			auto setInnerAngle(float angle) -> void;
			auto getInnerAngle() const -> float;
			auto setOuterAngle(float angle) -> void;
			auto getOuterAngle() const -> float;
			auto setOuterGain(float gain) -> void;
			auto getOuterGain() const -> float;
			// State manipulations
			auto getState() const -> SourceState;
			auto play() -> void;
			auto pause() -> void;
			auto stop() -> void;
			auto rewind() -> void;
		};
	} // namespace Audio
} // namespace HJUIK

#endif

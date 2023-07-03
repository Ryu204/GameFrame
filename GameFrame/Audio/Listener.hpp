#ifndef GAMEFRAME_AUDIO_LISTENER_HPP
#define GAMEFRAME_AUDIO_LISTENER_HPP

/*
    Represent the one and only listener in the world
    How Gain is calculated in INVERSE_CLAMPED model:
        ONE = AL_REFERENCE_DISTANCE // per-source attribute
        distance = clamp(distance, ONE, AL_MAX_DISTANCE)
        gain = ONE / (ONE + (distance - ONE) * ROLL_OFF) // ROLL_OFF is also a per-source attribute
*/

#include <AL/alc.h>
#include <AL/al.h>
#include "Utilize.hpp"

namespace HJUIK
{
    namespace Audio
    {
        // The way the output volume is calculated based on distance to listener
        enum class AttenuationModel : decltype(AL_INVERSE_DISTANCE_CLAMPED)
        {
            INVERSE = AL_INVERSE_DISTANCE,
            INVERSE_CLAMPED = AL_INVERSE_DISTANCE_CLAMPED,
            LINEAR = AL_LINEAR_DISTANCE,
            LINEAR_CLAMPED = AL_LINEAR_DISTANCE_CLAMPED,
            EXPONENT = AL_EXPONENT_DISTANCE,
            EXPONENT_CLAMPED = AL_EXPONENT_DISTANCE,
            NONE = AL_NONE
        };

        // Attenuation model relative to distance
        auto setAttenuationModel(AttenuationModel model) -> void;
        auto getAttenuationModel() -> AttenuationModel;
    } // namespace Audio
} // namespace HJUIK

#endif
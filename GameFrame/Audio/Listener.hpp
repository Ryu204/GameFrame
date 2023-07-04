#ifndef GAMEFRAME_AUDIO_LISTENER_HPP
#define GAMEFRAME_AUDIO_LISTENER_HPP

/*
    Represent the one and only listener in the world, and global queries
*/
/*
    How Gain is calculated in INVERSE_CLAMPED model:
    ONE = AL_REFERENCE_DISTANCE // per-source attribute
    distance = clamp(distance, ONE, AL_MAX_DISTANCE)
    gain = ONE / (ONE + (distance - ONE) * ROLL_OFF) // ROLL_OFF is also a per-source attribute
*/

#include <AL/alc.h>
#include <AL/al.h>
#include <tuple>
#include "Utilize.hpp"
#include "../Utilize/GLMTypedef.hpp"

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

        // Global state
        auto setAttenuationModel(AttenuationModel model) -> void;
        auto getAttenuationModel() -> AttenuationModel;
        auto setSpeedOfSound(float speed) -> void;
        auto getSpeedOfSound() -> float;
        auto setDopplerFactor(float dopplerFactor) -> void;
        auto getDopplerFactor() -> float;
        // Listener attributes
        auto setListenerPosition(Vector3f position) -> void;
        auto getListenerPosition() -> Vector3f;
        auto setListenerVelocity(Vector3f velocity) -> void;
        auto getListenerVelocity() -> Vector3f;
        auto setListenerGain(float gain) -> void;
        auto getListenerGain() -> float;

        /* 
        From OpenAL 1.1 Specs:
        AL_ORIENTATION is a pair of 3-tuples consisting of an 'at' vector and an 
        'up' vector, where the 'at' vector represents the 'forward' direction of 
        the listener and the orthogonal projection of the 'up' vector into the 
        subspace perpendicular to the 'at' vector represents the 'up' direction 
        for the listener. OpenAL expects two vectors that are linearly independent. 
        These vectors are not expected to be normalized. If the two vectors are
        linearly dependent, behavior is undefined.
        In another word, yAxis needs not to be perpendicular to zAxis, it just 
        needs to be linearly independent with zAxis.
        */
        auto setListenerOrientation(Vector3f zAxis, Vector3f yAxis) -> void;
        auto getListenerOrientation() -> std::tuple<Vector3f, Vector3f, Vector3f>;
    } // namespace Audio
} // namespace HJUIK

#endif
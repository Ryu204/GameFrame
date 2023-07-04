#include "Listener.hpp"

namespace HJUIK
{
    namespace Audio
    {
        // Set the model
        auto setAttenuationModel(AttenuationModel model) -> void
        {
            alCheck(alDistanceModel(static_cast<ALenum>(model)));
        }

        auto getAttenuationModel() -> AttenuationModel
        {
            ALint model = 0;
            alCheck(model = alGetInteger(AL_DISTANCE_MODEL));
            return static_cast<AttenuationModel>(model);
        }

        auto setSpeedOfSound(float speed) -> void
        {
            alCheck(alSpeedOfSound(speed));
        }

        auto getSpeedOfSound() -> float
        {
            ALfloat speed = 0;
            alCheck(speed = alGetFloat(AL_SPEED_OF_SOUND));
            return speed;
        }

        auto setDopplerFactor(float dopplerFactor) -> void
        {
            alCheck(alDopplerFactor(dopplerFactor));
        }

        auto getDopplerFactor() -> float
        {
            ALfloat factor = 0;
            alCheck(factor = alGetFloat(AL_DOPPLER_FACTOR));
            return factor;
        }

        auto setListenerPosition(Vector3f position) -> void
        {
            alCheck(alListener3f(AL_POSITION, position.x, position.y, position.z));
        }

        auto getListenerPosition() -> Vector3f
        {
            Vector3f position{};
            alCheck(alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z));
            return position;
        }

        auto setListenerVelocity(Vector3f velocity) -> void
        {
            alCheck(alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z));
        }

        auto getListenerVelocity() -> Vector3f
        {
            Vector3f velocity{};
            alCheck(alGetListener3f(AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z));
            return velocity;
        }

        auto setListenerGain(float gain) -> void
        {
            alCheck(alListenerf(AL_GAIN, gain));
        }

        auto getListenerGain() -> float
        {
            return alGet<float>(alGetListenerf, AL_GAIN);
        }

        // NOLINTBEGIN(*-avoid-c-arrays)
        auto setListenerOrientation(Vector3f zAxis, Vector3f yAxis) -> void
        {
            float arr[] = {zAxis.x, zAxis.y, zAxis.z, yAxis.x, yAxis.y, yAxis.z};
            alCheck(alListenerfv(AL_ORIENTATION, static_cast<const float*>(arr)));
        }

        auto getListenerOrientation() -> std::tuple<Vector3f, Vector3f, Vector3f>
        {
            // NOLINTBEGIN(*-magic-numbers)
            float arr[6];
            alCheck(alGetListenerfv(AL_ORIENTATION, static_cast<float*>(arr)));
            Vector3f zAxis{arr[0], arr[1], arr[2]};
            Vector3f yAxis{arr[3], arr[4], arr[5]};
            // NOLINTEND(*-magic-numbers)
            Vector3f xAxis = glm::cross(yAxis, zAxis);
            return {xAxis, yAxis, zAxis};
        }
        // NOLINTEND(*-avoid-c-arrays)
    } // namespace Audio
} // namespace HJUIK
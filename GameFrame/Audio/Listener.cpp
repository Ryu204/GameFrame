#include "Listener.hpp"

namespace
{
    // NOLINTBEGIN(*-non-const-global-variables)
    HJUIK::Audio::AttenuationModel CURRENT_MODEL = HJUIK::Audio::AttenuationModel::INVERSE_CLAMPED;
    // NOLINTEND(*-non-const-global-variables)
} // namespace

namespace HJUIK
{
    namespace Audio
    {
        // Set the model
        auto setAttenuationModel(AttenuationModel model) -> void
        {
            alCheck(alDistanceModel(static_cast<ALenum>(model)));
            CURRENT_MODEL = model;
        }
        auto getAttenuationModel() -> AttenuationModel
        {
            return CURRENT_MODEL;
        }
    } // namespace Audio
} // namespace HJUIK
#include <memory>
#include <mutex>
#include "AudioDevice.hpp"
#include "ContextBasedWrapper.hpp"

namespace
{
    // NOLINTBEGIN(*-non-const-global-variables)
    std::unique_ptr<HJUIK::Audio::AudioDevice> GLOBAL_DEVICE;
    std::recursive_mutex GLOBAL_MUTEX;
    std::size_t COUNT;
    // NOLINTEND(*-non-const-global-variables)
} // namespace

namespace HJUIK
{
    namespace Audio
	{
        // Derived class require that at least one device is available at its creation
        ContextBasedWrapper::ContextBasedWrapper()
        {
            std::lock_guard lock(GLOBAL_MUTEX);
            // If no device is created yet
            if (COUNT == 0)
            {
                GLOBAL_DEVICE = std::make_unique<AudioDevice>();
            }
            COUNT++;
        }
        
        ContextBasedWrapper::~ContextBasedWrapper()
        {

            std::lock_guard lock(GLOBAL_MUTEX);
            COUNT--;
            if (COUNT == 0)
            {
                GLOBAL_DEVICE.reset();
            }
        }
    } // namespace Audio
} // namespace HJUIK

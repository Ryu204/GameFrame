#include "Error.hpp"
#include <AL/al.h>
#include <AL/alc.h>

namespace HJUIK
{
    namespace Audio
    {
        namespace detail
        {
            auto alCheckLastErr(const char* file, unsigned int line, const char* expr) -> void
            {
                ALenum errorCode = alGetError();

                if (errorCode != AL_NO_ERROR)
                {
                    std::string description = "No description";
                    std::string error = "Unknown";

                    // Decode the error code
                    switch (errorCode)
                    {
                    case AL_INVALID_NAME:
                        error = "AL_INVALID_NAME";
                        description = "A bad name (ID) has been specified";
                        break;
                    case AL_INVALID_ENUM:
                        error = "AL_INVALID_ENUM";
                        description = "An unacceptable value has been specified for an enumerated argument";
                        break;
                    case AL_INVALID_VALUE:
                        error = "AL_INVALID_VALUE";
                        description = "A numeric argument is out of range";
                        break;
                    case AL_INVALID_OPERATION:
                        error = "AL_INVALID_OPERATION";
                        description = "The specified operation is not allowed in the current state";
                        break;
                    case AL_OUT_OF_MEMORY:
                        error = "AL_OUT_OF_MEMORY";
                        description = "There is not enough memory left to execute the command";
                        break;
                    default:
                        break;
                    }

                    throw std::runtime_error(
                        "OpenAL: " + error + " - " + description + "\nIn file " 
                        + std::string(file) + ", line " + std::to_string(line) + ". Details:\n" 
                        + std::string(expr));
                }
            }

            auto alcCheckLastErr(const char* file, unsigned int line, const char* expr, ALCdevice* device) -> void
            {
                ALCenum errorCode = alcGetError(device);

                if (errorCode != ALC_NO_ERROR)
                {
                    std::string description = "No description";
                    std::string error = "Unknown";

                    // Decode the error code
                    switch (errorCode)
                    {
                    case ALC_INVALID_VALUE:
                        error = "ALC_INVALID_VALUE";
                        description = "A numeric argument is out of range";
                        break;
                    case ALC_INVALID_ENUM:
                        error = "ALC_INVALID_ENUM";
                        description = "An unacceptable value has been specified for an enumerated argument";
                        break;
                    case ALC_INVALID_CONTEXT:
                        error = "ALC_INVALID_CONTEXT";
                        description = "A bad context was passed as argument";
                        break;
                    case ALC_INVALID_DEVICE:
                        error = "ALC_INVALID_DEVICE";
                        description = "A bad device was passed as argument";
                        break;
                    case ALC_OUT_OF_MEMORY:
                        error = "ALC_OUT_OF_MEMORY";
                        description = "There is not enough memory left to execute the command";
                        break;
                    default:
                        break;
                    }

                    throw std::runtime_error(
                        "OpenAL: " + error + " - " + description + "\nIn file " 
                        + std::string(file) + ", line " + std::to_string(line) + ". Details:\n" 
                        + std::string(expr));
                }
            }
        } // namespace detail
    } // namespace Audio
} // namespace HJUIK
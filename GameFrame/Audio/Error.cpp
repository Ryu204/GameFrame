#include "Error.hpp"
#include <AL/al.h>

namespace HJUIK
{
    namespace Audio
    {
        auto alCheckLastErr(const char* file, unsigned int line, const char* expr) -> void
        {
            ALenum errorCode = alGetError();

            if (errorCode != AL_NO_ERROR)
            {
                std::string description = "No description";

                // Decode the error code
                switch (errorCode)
                {
                case AL_INVALID_NAME:
                    description = "A bad name (ID) has been specified";
                    break;
                case AL_INVALID_ENUM:
                    description = "An unacceptable value has been specified for an enumerated argument";
                    break;
                case AL_INVALID_VALUE:
                    description = "A numeric argument is out of range";
                    break;
                case AL_INVALID_OPERATION:
                    description = "The specified operation is not allowed in the current state";
                    break;
                case AL_OUT_OF_MEMORY:
                    description = "There is not enough memory left to execute the command";
                    break;
                default:
                    break;
                }

                throw std::runtime_error(
                    "OpenAL: " + description + "\nIn file " 
                    + std::string(file) + ", line " + std::to_string(line) + "\nDetails: " 
                    + std::string(expr));
            }
        }
    } // namespace Audio
} // namespace HJUIK
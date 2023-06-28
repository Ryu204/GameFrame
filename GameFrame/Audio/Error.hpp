#ifndef GAMEFRAME_AUDIO_ERROR_HPP
#define GAMEFRAME_AUDIO_ERROR_HPP

#include "../Utilize/CallAssert.hpp"
#include <AL/al.h>

/*
    From here: https://github.com/SFML/SFML/blob/2.6.0/src/SFML/Audio/ALCheck.hpp
*/

namespace HJUIK
{
    namespace Audio
    {   
        namespace detail
        {
            auto alCheckLastErr(const char* file, unsigned int line, const char* expr) -> void;
        } // namespace detail
    } // namespace Audio
} // namespace HJUIK


// NOLINTBEGIN(*-macro-usage)
#ifndef NDEBUG
    #define alCheck(expr) do {expr; HJUIK::Audio::detail::alCheckLastErr(__FILE__, __LINE__, #expr);} while (false);
#else
    #define alCheck(expr) {expr};
#endif // NDEBUG
// NOLINTEND(*-macro-usage)

#endif
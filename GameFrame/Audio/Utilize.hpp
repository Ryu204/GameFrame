#ifndef GAMEFRAME_AUDIO_UTILIZE_HPP
#define GAMEFRAME_AUDIO_UTILIZE_HPP

#include "../Utilize/CallAssert.hpp"
#include <AL/al.h>
#include <utility>
#include <AL/alc.h>

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
            auto alcCheckLastErr(const char* file, unsigned int line, const char* expr, ALCdevice* device) -> void;
        } // namespace detail

// NOLINTBEGIN(*-macro-usage)
#ifndef NDEBUG
    #define alCheck(expr) do {expr; HJUIK::Audio::detail::alCheckLastErr(__FILE__, __LINE__, #expr);} while (false);
#else
    #define alCheck(expr) {expr};
#endif // NDEBUG

#ifndef NDEBUG
    #define alcCheck(expr, devicePtr) do {expr; HJUIK::Audio::detail::alcCheckLastErr(__FILE__, __LINE__, #expr, devicePtr);} while (false);
#else
    #define alcCheck(expr, devicePtr) {expr};
#endif // NDEBUG
// NOLINTEND(*-macro-usage)

        template <typename Value, typename Func, typename... Args>
        auto alGet(Func&& alGetFunc, Args&&... args) -> Value
        {
            auto val = static_cast<Value>(0);
            alCheck(std::forward<Func>(alGetFunc)(std::forward<Args>(args)..., &val));
            return val;
        }

        template <typename Value, typename Func, typename... Args>
        auto alGen(Func&& alGenFunc, Args&&... args) -> Value
        {
            return alGet<Value>(std::forward<Func>(alGenFunc), std::forward<Args>(args)..., 1);
        }
    } // namespace Audio
} // namespace HJUIK

#endif
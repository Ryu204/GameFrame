#ifndef GAMEFRAME_UTILIZE_CALL_ASSERT_HPP
#define GAMEFRAME_UTILIZE_CALL_ASSERT_HPP

#include <string>
#include <exception>
#include <stdexcept>

/*
    Utilize methods to check a conditions and execute corresponding actions
*/

namespace HJUIK
{
    namespace Utilize
    {
        template <typename Type>
        // Check if 'value' is non-zero or else throw std::runtime_error
        inline auto checkNonZero(Type&& value, const char* errorMessage) -> decltype(auto)
        {
            if (value == 0)
            {
                throw std::runtime_error(errorMessage);
            }

            return std::forward<Type>(value);
        }

        template <typename Type>
        // Check if 'value' is non-zero or else throw std::runtime_error
        inline auto checkNonZero(Type&& value, const std::string& errorMessage) -> decltype(auto)
        {
            if (value == 0)
            {
                throw std::runtime_error(errorMessage);
            }

            return std::forward<Type>(value);
        }
    } // namespace Utilize
} // namespace HJUIK

#endif
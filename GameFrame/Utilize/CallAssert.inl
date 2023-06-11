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
        namespace detail
        {
            namespace Assert
            {
                // We have the right to use inefficient code here because the program is gonna crash soon
                template<typename T>
                void recursivePrint(T arg1)
                {
                    std::cout << arg1;
                }

                template <typename T1, typename... Ts>
                void recursivePrint(T1 arg1, Ts... args)
                {
                    std::cout << arg1;
                    recursivePrint(args...);
                }
            } // namespace Assert
        } // namespace detail

        template <typename Type>
        // Check if 'value' is non-zero or else throw std::runtime_error
        inline auto throwIfZero(Type&& value, const char* errorMessage) -> decltype(auto)
        {
            if (value == 0)
            {
                throw std::runtime_error(errorMessage);
            }

            return std::forward<Type>(value);
        }

        template <typename Type>
        // Check if 'value' is non-zero or else throw std::runtime_error
        inline auto throwIfZero(Type&& value, const std::string& errorMessage) -> decltype(auto)
        {
            if (value == 0)
            {
                throw std::runtime_error(errorMessage);
            }

            return std::forward<Type>(value);
        }
    } // namespace Utilize
} // namespace HJUIK
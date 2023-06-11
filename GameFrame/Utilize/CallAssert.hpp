#ifndef GAMEFRAME_UTILIZE_CALL_ASSERT_HPP
#define GAMEFRAME_UTILIZE_CALL_ASSERT_HPP

#include <string>
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <thread>

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
                template<typename T>
                void recursivePrint(T arg1);

                template <typename T1, typename... Ts>
                void recursivePrint(T1 arg1, Ts... args);
            } // namespace Assert
        } // namespace detail

		// Check if 'value' is non-zero and return it or else throw std::runtime_error
		template <typename Type>
		inline auto throwIfZero(Type&& value, const char* errorMessage) -> decltype(auto);

		// Check if 'value' is non-zero and return it or else throw std::runtime_error
		template <typename Type>
		inline auto throwIfZero(Type&& value, const std::string& errorMessage) -> decltype(auto);
	} // namespace Utilize
} // namespace HJUIK

// Assertion. The '...' parts are messages.
#ifndef NDEBUG // if built in debug mode
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HJUIK_ASSERT(condition, ...){\
            if ((condition) == 0)\
            {\
			    HJUIK::Utilize::detail::Assert::recursivePrint(\
                    "HJUIK: Assertion failed at file ", __FILE__", line ", __LINE__,\
                    ". Details:\n", __VA_ARGS__, '\n');\
                /* NOLINTNEXTLINE(concurrency-mt-unsafe) */\
                exit(-1);\
			}\
		}
#else
        // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
        #define HJUIK_ASSERT(...) {}
#endif // NDEBUG

#include "CallAssert.inl"

#endif
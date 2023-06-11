#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

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
				template <typename... Args>
				void recursivePrint(Args&&... args)
				{
					// https://stackoverflow.com/questions/43070062/using-fold-expressions-to-print-all-variadic-arguments-with-newlines-inbetween
					(std::cout << ... << std::forward<Args>(args));
				}
			} // namespace Assert
		}	  // namespace detail

		template <typename Type>
		// Check if 'value' is non-zero or else throw std::runtime_error
		inline auto throwIfZero(Type&& value, const char* errorMessage) -> decltype(auto)
		{
			if (value == 0) {
				throw std::runtime_error(errorMessage);
			}

			return std::forward<Type>(value);
		}

		template <typename Type>
		// Check if 'value' is non-zero or else throw std::runtime_error
		inline auto throwIfZero(Type&& value, const std::string& errorMessage) -> decltype(auto)
		{
			if (value == 0) {
				throw std::runtime_error(errorMessage);
			}

			return std::forward<Type>(value);
		}
#ifndef NDEBUG
		template <typename Type, typename LINE_TYPE, typename FILE_TYPE, typename... Args>
		inline auto assertIfZero(Type&& value, LINE_TYPE line, FILE_TYPE file, Args&&... args) -> void
		{
			if (value == 0) {
				Utilize::detail::Assert::recursivePrint(
					"HJUIK: Assertion failed at file:", file, ", line:", line, ". Details:\n", args..., '\n');
				exit(-1);
			}
		}
#else
		template <typename Type, typename LINE_TYPE, typename FILE_TYPE, typename... Args>
		inline auto assertIfZero(Type&& value, LINE_TYPE line, FILE_TYPE file, Args&&... args) -> void
		{
			// Literally does nothing
		}
#endif // NDEBUG
	}  // namespace Utilize
} // namespace HJUIK

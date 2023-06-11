#ifndef GAMEFRAME_UTILIZE_CALL_ASSERT_HPP
#define GAMEFRAME_UTILIZE_CALL_ASSERT_HPP

#include <string>
#include <iostream>

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
                template<typename... Args>
                void recursivePrint(Args&&... args);
            } // namespace Assert
        } // namespace detail

		// Check if 'value' is non-zero or else throw std::runtime_error
		template <typename Type>
		inline auto throwIfZero(Type&& value, const char* errorMessage) -> decltype(auto);

		// Check if 'value' is non-zero or else throw std::runtime_error
		template <typename Type>
		inline auto throwIfZero(Type&& value, const std::string& errorMessage) -> decltype(auto);

        template <typename Type, typename LINE_TYPE, typename FILE_TYPE, typename... Args>
        inline auto assertIfZero(Type&& value, LINE_TYPE line, FILE_TYPE file, Args&&... args) -> void;
	} // namespace Utilize
} // namespace HJUIK

#include "CallAssert.inl"

#endif

#ifndef GAMEFRAME_UTILIZE_VARIANT_HELPER_HPP
#define GAMEFRAME_UTILIZE_VARIANT_HELPER_HPP

/*
    Defines method to get a std::variant index by type (via templates)
*/

#include <variant>

namespace HJUIK
{
    namespace Utilize
    {
        // Implementation details, please ignore
        namespace detail
        {
            namespace Variant
            {
                template <typename Type>
                struct Tag { };

                template <typename Type, typename T>
                struct IndexGetter { };

                // Usage: IndexGetter<Type, std::variant<...>>::value
                template <typename Type, typename... Ts>
                struct IndexGetter<Type, std::variant<Ts...>>
                    : std::integral_constant<std::size_t, std::variant<Tag<Ts>...>(Tag<Type>()).index()>
                { };
            } // namespace Variant
        } // namespace detail

        template <typename T, typename V>
        class VariantHelper { };

        // Utilize working with std::variant
        template <typename Type, typename... Ts>
        class VariantHelper<Type, std::variant<Ts...>>
        {
        public:
            // Return the index of Type inside std::variant<Ts...>
            static constexpr std::size_t index();
        }; 

        template <typename Type, typename... Ts>
        constexpr std::size_t VariantHelper<Type, std::variant<Ts...>>::index()
        {
            return detail::Variant::IndexGetter<Type, std::variant<Ts...>>::value;
        }
    } // namespace Utilize
} // namespace HJUIK

#endif
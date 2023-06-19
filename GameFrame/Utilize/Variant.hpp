#ifndef GAMEFRAME_UTILIZE_VARIANT_HPP
#define GAMEFRAME_UTILIZE_VARIANT_HPP

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
				struct Tag {
				};

				template <typename Type, typename T>
				struct IndexGetter {
				};

				// Usage: IndexGetter<Type, std::variant<...>>::value
				template <typename Type, typename... Ts>
				struct IndexGetter<Type, std::variant<Ts...>>
					: std::integral_constant<std::size_t, std::variant<Tag<Ts>...>(Tag<Type>()).index()> {
				};
			} // namespace Variant
		} // namespace detail

		template <typename T, typename V>
		class VariantHelper
		{
		};

		// Utilize working with std::variant
		template <typename Type, typename... Ts>
		class VariantHelper<Type, std::variant<Ts...>>
		{
		public:
			// Return the index of Type inside std::variant<Ts...>
			static constexpr auto index() -> std::size_t;
		};

		template <typename Type, typename... Ts>
		constexpr auto VariantHelper<Type, std::variant<Ts...>>::index() -> std::size_t
		{
			return detail::Variant::IndexGetter<Type, std::variant<Ts...>>::value;
		}


        // https://en.cppreference.com/w/cpp/utility/variant/visit
        template <typename... Fs>
        // NOLINTNEXTLINE(*-multiple-inheritance)
        struct VariantVisitor : Fs... {using Fs::operator ()...;};
        template <typename... Fs>
        VariantVisitor(Fs...) -> VariantVisitor<Fs...>;
	} // namespace Utilize
} // namespace HJUIK

#endif
#ifndef GAMEFRAME_UTILIZE_RESOURCEHOLDER_HPP
#define GAMEFRAME_UTILIZE_RESOURCEHOLDER_HPP

/*
	This class is used to manage instances
	of heavy resource types. Each instance is
	identified by a key.
*/

#include <memory>
#include <unordered_map>

namespace HJUIK
{
	namespace Utilize
	{
		// Efficient data structure to store heavy-weight data
		// Args: constructor parameter
		template <typename Key, typename Val>
		class ResourceHolder
		{
		public:
			ResourceHolder();
			// Create new resource. Args... are resource's constructor paramter(s)
			template <typename... Args>
			auto create(Key key, Args... params) -> void;
			// Create new default resource
			auto create(Key key) -> void;
			// Retrieve the resource reference
			auto get(Key key) -> Val&;

		private:
			std::unordered_map<Key, std::unique_ptr<Val>> mContainer;
		};
	} // namespace Utilize
} // namespace HJUIK

#include "ResourceHolder.inl"

#endif

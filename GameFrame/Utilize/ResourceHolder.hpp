#ifndef __UTILIZE_RESOURCEHOLDER_HPP__
#define __UTILIZE_RESOURCEHOLDER_HPP__

/*
	This class is used to manage instances 
	of heavy resource types. Each instance is 
	identified by a key.
*/

#include <unordered_map>
#include <memory>

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
			void create(Key key, Args... params);
			// Create new default resource
			void create(Key key);
			// Retrieve the resource reference
			Val& get(Key key);
		private:
			std::unordered_map<Key, std::unique_ptr<Val>> mContainer;
		};
	}
}

#include "ResourceHolder.inl"

#endif



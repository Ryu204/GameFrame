#ifndef __UTILIZE_RESOURCEHOLDER_HPP__
#define __UTILIZE_RESOURCEHOLDER_HPP__

/*
	This class is used to manage many instanes 
	of a heavy resource type. Each instance is 
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

			template <typename... Args>
			void create(Key key, Args... params);
			Val& get(Key key);
		private:

			std::unordered_map<Key, std::unique_ptr<Val>> mContainer;
		};
	}
}

#include "ResourceHolder.inl"

#endif



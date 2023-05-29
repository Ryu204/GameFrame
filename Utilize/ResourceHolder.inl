#include <cassert>

namespace HJUIK
{
	namespace Utilize
	{
		template <typename Key, typename Val>
		ResourceHolder<Key, Val>::ResourceHolder()
			: mContainer()
		{ }

		template <typename Key, typename Val>
		template <typename... Args>
		void ResourceHolder<Key, Val>::create(Key key, Args... params)
		{
			mContainer[key] = std::make_unique<Val>(params...);
		}

		template <typename Key, typename Val>
		void ResourceHolder<Key, Val>::create(Key key)
		{
			mContainer[key] = std::make_unique<Val>();
		}

		template <typename Key, typename Val>
		Val& ResourceHolder<Key, Val>::get(Key key)
		{
			return *mContainer[key];
		}
	}
}
namespace HJUIK::Utilize
{
	template <typename Key, typename Val>
	ResourceHolder<Key, Val>::ResourceHolder()
		: mContainer()
	{ }

	template <typename Key, typename Val>
	template <typename... Args>
	auto ResourceHolder<Key, Val>::create(Key key, Args... params) -> void
	{
		mContainer[key] = std::make_unique<Val>(params...);
	}

	template <typename Key, typename Val>
	auto ResourceHolder<Key, Val>::create(Key key) -> void
	{
		mContainer[key] = std::make_unique<Val>();
	}

	template <typename Key, typename Val>
	auto ResourceHolder<Key, Val>::get(Key key) -> Val&
	{
		return *mContainer[key];
	}
}
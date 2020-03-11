// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
};

ExpandableHashMap::ExpandableHashMap(double maximumLoadFactor)
{
}

ExpandableHashMap::~ExpandableHashMap()
{
}

void ExpandableHashMap::reset()
{
}

int ExpandableHashMap::size() const
{
    return -999;  // Delete this line and implement this function correctly
}

void ExpandableHashMap::associate(const KeyType& key, const ValueType& value)
{
}

const ValueType* ExpandableHashMap::find(const KeyType& key) const
{
    return nullptr;  // Delete this line and implement this function correctly
}


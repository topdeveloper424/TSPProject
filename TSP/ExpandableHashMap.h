// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#include <iostrem>
#include <functional>
#include <list>

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

	void print()
	{
		for (int i = 0; i < maxBuckets; i++)
		{
			Bucket* p = m_hash[i];
			while (p != nullptr)
			{
				std::cout << p->k.latitudeText << " " << p->k.longitudeText << std::endl;
				p = p->next;
			}
		}
	}

private:
	struct Bucket
	{
		Bucket(KeyType key, ValueType value) :k(key), v(value), next(nullptr) {}
		KeyType k;
		ValueType v;
		Bucket* next;
	};
	unsigned int getBucketNum(const KeyType& k) const
	{
		unsigned int hasher(const KeyType & key); // prototype
		unsigned int h = hasher(k);
		return h;
	}
	double maxLoad;
	int numBuckets;
	int numElements;
	int maxBuckets;
	bool inMap(KeyType key);
	Bucket** m_hash;
};

template<typename KeyType, typename ValueType>
bool ExpandableHashMap<KeyType, ValueType>::inMap(KeyType key)
{
	unsigned int h = getBucketNum(key);
	unsigned int bucketNum = h % maxBuckets;
	if (m_hash[bucketNum] == nullptr)
		return false;
	else
	{
		Bucket* p = m_hash[bucketNum];
		while (p != nullptr)
		{
			if (p->k == key)
				return true;
			p = p->next;
		}
	}
	return false;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
	maxLoad = maximumLoadFactor;
	maxBuckets = 8;
	numBuckets = 0;
	numElements = 0;
	m_hash = new Bucket * [maxBuckets] {nullptr};
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
	Bucket* p = m_hash[0];
	while (p != nullptr) {
		Bucket* temp = p;
		p = p->next;
		delete temp;
	}
	delete p;
	delete[] m_hash;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
	Bucket* p = m_hash[0];
	while (p != nullptr) {
		Bucket* temp = p;
		p = p->next;
		delete temp;
	}
	delete p;
	delete[] m_hash;
	numBuckets = 0;
	maxBuckets = 8;
	m_hash = new Bucket * [maxBuckets] {nullptr};
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
	return numElements;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
	unsigned int h = getBucketNum(key);
	unsigned int bucketNum = h % maxBuckets;
	if (inMap(key))
	{
		Bucket* p = m_hash[bucketNum];
		while (p != nullptr)
		{
			if (p->k == key)
			{
				p->v = value;
				break;
			}
			else
				p = p->next;
		}
	}
	else
	{
		Bucket* b = new Bucket(key, value);
		if (m_hash[bucketNum] == nullptr)
			m_hash[bucketNum] = b;
		else
			m_hash[bucketNum]->next = b;
		numBuckets++;
		numElements++;
	}
	if ((double)numElements / maxBuckets > maxLoad)
	{
		Bucket** p;
		p = new Bucket *[maxBuckets * 2]{ nullptr };
		for (int i = 0; i < maxBuckets; i++)
		{
			while (m_hash[i] != nullptr)
			{
				KeyType curK = m_hash[i]->k;
				ValueType curV = m_hash[i]->v;
				Bucket* buck = new Bucket(curK, curV);
				unsigned int newH = getBucketNum(curK);
				unsigned int newBucket = newH % (maxBuckets * 2);
				if (p[newBucket] == nullptr)
					p[newBucket] = buck;
				else
					p[newBucket]->next = buck;
				m_hash[i] = m_hash[i]->next;
			}
		}
		Bucket* d = m_hash[0];
		while (d != nullptr) {
			Bucket* temp = d;
			d = d->next;
			delete temp;
		}
		delete d;
		delete[] m_hash;
		m_hash = p;
		maxBuckets *= 2;
	}
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
	unsigned int h = getBucketNum(key);
	unsigned int bucketNum = h % maxBuckets;
	if (numBuckets == 0)
		return nullptr;
	else if (m_hash[bucketNum] != nullptr)
	{
		Bucket* cur = m_hash[bucketNum];
		while (cur != nullptr)
		{
			if (cur->k == key)
			{
				return &cur->v;
			}
			cur = cur->next;
		}
	}
	return nullptr;
}
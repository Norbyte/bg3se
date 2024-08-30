#pragma once

#include <cstdint>

BEGIN_SE()

unsigned int GetNearestMultiHashMapPrime(unsigned int num);

template <class T>
inline uint64_t SparseHashMapHash(T const& v);

// Special hashing needed for FixedStrings in the new hash table

template <class T>
inline uint64_t HashMapHash(T const& v)
{
	return Hash(v);
}

template <>
inline uint64_t HashMapHash<FixedString>(FixedString const& v)
{
	return v.GetHash();
}

template <class T>
class HashSet
{
public:
	using value_type = T;
	using reference = T&;
	using const_reference = T const&;
	using iterator = ContiguousIterator<T>;
	using const_iterator = ContiguousConstIterator<T>;
	using difference_type = int32_t;
	using size_type = uint32_t;

	HashSet() noexcept
	{}

	HashSet(HashSet const& other)
		: HashKeys(other.HashKeys), NextIds(other.NextIds), Keys(other.Keys)
	{}

	HashSet(HashSet&& other) noexcept
		: HashKeys(std::move(other.HashKeys)), NextIds(std::move(other.NextIds)), Keys(std::move(other.Keys))
	{}

	~HashSet()
	{}

	HashSet& operator =(HashSet const& other)
	{
		HashKeys = other.HashKeys;
		NextIds = other.NextIds;
		Keys = other.Keys;
		return *this;
	}

	HashSet& operator =(HashSet&& other) noexcept
	{
		if (this != &other) {
			HashKeys = std::move(other.HashKeys);
			NextIds = std::move(other.NextIds);
			Keys = std::move(other.Keys);
		}

		return *this;
	}

	inline uint32_t size() const
	{
		return Keys.size();
	}

	inline bool empty() const
	{
		return Keys.empty();
	}

	Array<T> const& keys() const
	{
		return Keys;
	}

	StaticArray<int32_t> const& hash_keys() const
	{
		return HashKeys;
	}

	Array<int32_t> const& next_ids() const
	{
		return NextIds;
	}

	void clear()
	{
		std::fill(HashKeys.begin(), HashKeys.end(), -1);
		NextIds.clear();
		Keys.clear();
	}

	int find_index(T const& key) const
	{
		if (HashKeys.size() == 0) return -1;

		auto keyIndex = HashKeys[HashMapHash(key) % HashKeys.size()];
		while (keyIndex >= 0) {
			if (Keys[keyIndex] == key) return keyIndex;
			keyIndex = NextIds[keyIndex];
		}

		return -1;
	}

	int insert(T const& key)
	{
		auto index = find_index(key);
		if (index != -1) {
			return index;
		}

		int keyIdx = (int)Keys.size();
		Keys.Add(key);
		NextIds.Add(-1);

		auto desiredSize = Keys.size() + (Keys.size() >> 1);
		if (HashKeys.size() >= desiredSize) {
			InsertToHashMap(key, keyIdx);
		} else {
			ResizeHashMap(desiredSize);
		}

		return keyIdx;
	}

	ContiguousIterator<T> begin()
	{
		return Keys.begin();
	}

	ContiguousConstIterator<T> begin() const
	{
		return Keys.begin();
	}

	ContiguousIterator<T> end()
	{
		return Keys.end();
	}

	ContiguousConstIterator<T> end() const
	{
		return Keys.end();
	}

	ContiguousIterator<T> find(T const& key) const
	{
		auto idx = find_index(key);
		return idx != -1 ? (Keys.begin() + idx) : Keys.end();
	}

	ContiguousIterator<T> find(T const& key)
	{
		auto idx = find_index(key);
		return idx != -1 ? (Keys.begin() + idx) : Keys.end();
	}

	bool remove(T const& key)
	{
		auto idx = find_index(key);
		if (idx != -1) {
			removeAt(idx);
			return true;
		} else {
			return false;
		}
	}

	bool contains(T const& key) const
	{
		return find_index(key) != -1;
	}

protected:
	StaticArray<int32_t> HashKeys;
	Array<int32_t> NextIds;
	Array<T> Keys;

	uint32_t bucketFromIndex(int32_t index)
	{
		while (index >= 0) {
			index = NextIds[index];
		}

		assert(index < -1);
		return (uint32_t)(-2 - index);
	}

	void swapKeyInHashTable(uint32_t bucket, int32_t indexToRemove, int32_t nextIndex)
	{
		auto index = HashKeys[bucket];
		if (indexToRemove == index) {
			HashKeys[bucket] = nextIndex;
		} else {
			int32_t lastIndex = -1;
			do {
				assert(index >= 0);
				lastIndex = index;
				index = NextIds[index];
			} while (index != indexToRemove);

			NextIds[lastIndex] = nextIndex;
		}
	}

	void removeAt(int32_t index)
	{
		auto bucketIndex = bucketFromIndex(index);
		auto nextIndex = NextIds[index];
		swapKeyInHashTable(bucketIndex, index, nextIndex);
		Keys.remove_at(index);
		auto followUpIndex = NextIds.pop_last();
		auto removedIndex = NextIds.size();

		if (index != removedIndex) {
			NextIds[index] = followUpIndex;
			auto removedBucketIndex = bucketFromIndex(followUpIndex);
			swapKeyInHashTable(removedBucketIndex, removedIndex, index);
		}
	}

private:
	void InsertToHashMap(T const& key, int keyIdx)
	{
		auto bucket = (uint32_t)(HashMapHash(key) % HashKeys.size());
		auto prevKeyIdx = HashKeys[bucket];
		if (prevKeyIdx < 0) {
			prevKeyIdx = -2 - (int)bucket;
		}

		NextIds[keyIdx] = prevKeyIdx;
		HashKeys[bucket] = keyIdx;
	}

	void ResizeHashMap(unsigned int newSize)
	{
		auto numBuckets = GetNearestMultiHashMapPrime(newSize);

		HashKeys = StaticArray<int32_t>(numBuckets, -1);
		for (unsigned k = 0; k < Keys.size(); k++) {
			InsertToHashMap(Keys[k], k);
		}
	}
};

template <class T>
struct VirtualMultiHashSet : public HashSet<T>
{
	virtual inline void Dummy() {}
};

template <class TKey, class TValue>
class HashMap : private HashSet<TKey>
{
public:
	class ConstIterator
	{
	public:
		ConstIterator(HashMap const* map)
			: Map(map), Index(0)
		{}
			
		ConstIterator(HashMap const* map, int index)
			: Map(map), Index(index)
		{}

		ConstIterator operator ++ ()
		{
			ConstIterator it(Map, Index);
			Index++;
			return it;
		}

		ConstIterator& operator ++ (int)
		{
			++Index;
			return *this;
		}

		bool operator == (ConstIterator const& it)
		{
			return it.Map == Map && it.Index == Index;
		}

		bool operator != (ConstIterator const& it)
		{
			return it.Map != Map || it.Index != Index;
		}

		TKey const& Key() const
		{
			return Map->Keys[Index];
		}

		TValue const& Value() const
		{
			return Map->Values[Index];
		}

		ConstIterator& operator * ()
		{
			return *this;
		}

		ConstIterator* operator -> ()
		{
			return *this;
		}

		inline operator bool() const
		{
			return Index != Map->Keys.size();
		}

		inline bool operator !() const
		{
			return Index == Map->Keys.size();
		}

	private:
		HashMap const* Map;
		int32_t Index;
	};

	class Iterator
	{
	public:
		Iterator(HashMap * map)
			: Map(map), Index(0)
		{}
			
		Iterator(HashMap * map, int index)
			: Map(map), Index(index)
		{}

		Iterator operator ++ ()
		{
			Iterator it(Map, Index);
			Index++;
			return it;
		}

		Iterator& operator ++ (int)
		{
			++Index;
			return *this;
		}

		bool operator == (Iterator const& it)
		{
			return it.Map == Map && it.Index == Index;
		}

		bool operator != (Iterator const& it)
		{
			return it.Map != Map || it.Index != Index;
		}

		TKey const & Key() const
		{
			return Map->Keys[Index];
		}

		TValue & Value() const
		{
			return Map->Values[Index];
		}

		Iterator& operator * ()
		{
			return *this;
		}

		Iterator* operator -> ()
		{
			return this;
		}

		inline operator bool() const
		{
			return Index != Map->Keys.size();
		}

		inline bool operator !() const
		{
			return Index == Map->Keys.size();
		}

	private:
		HashMap* Map;
		int32_t Index;
	};

	HashMap() noexcept
	{}

	HashMap(HashMap const& other)
		: HashSet<TKey>(other)
	{
		Values.copy_from(other.Values, 0, other.Keys.size());
	}

	HashMap(HashMap&& other) noexcept
		: HashSet<TKey>(std::move(other)),
		Values(std::move(other.Values))
	{}

	~HashMap()
	{
		Values.clear(this->Keys.size());
	}

	HashMap& operator =(HashMap const& other)
	{
		auto oldSize = this->Keys.size();
		HashSet<TKey>::operator =(other);
		Values.copy_from(other.Values, oldSize, other.Keys.size());

		return *this;
	}

	HashMap& operator =(HashMap&& other) noexcept
	{
		HashSet<TKey>::operator =(std::move(other));
		Values = std::move(other.Values);

		return *this;
	}

	inline uint32_t size() const
	{
		return this->Keys.size();
	}

	inline bool empty() const
	{
		return this->Keys.empty();
	}

	Array<TKey> const& keys() const
	{
		return this->Keys;
	}

	std::span<TValue> values()
	{
		return std::span(Values.raw_buf(), Values.raw_buf() + this->Keys.size());
	}

	std::span<TValue const> values() const
	{
		return std::span(Values.raw_buf(), Values.raw_buf() + this->Keys.size());
	}

	UninitializedStaticArray<TValue>& raw_values()
	{
		return Values;
	}

	UninitializedStaticArray<TValue> const& raw_values() const
	{
		return Values;
	}

	void clear()
	{
		Values.clear(this->Keys.size());
		HashSet<TKey>::clear();
	}

	TValue* set(TKey const& key, TValue&& value)
	{
		auto index = this->insert(key);
		if (Values.size() < this->Keys.capacity()) {
			Values.resize(this->Keys.capacity(), index, index);
		}

		return new (&Values[index]) TValue(std::move(value));
	}

	TValue* set(TKey const& key, TValue const& value)
	{
		auto index = this->insert(key);
		if (Values.size() < this->Keys.capacity()) {
			Values.resize(this->Keys.capacity(), index, index);
		}

		return new (&Values[index]) TValue(value);
	}

	TValue* add_key(TKey const& key)
	{
		auto index = this->insert(key);
		if (Values.size() < this->Keys.capacity()) {
			Values.resize(this->Keys.capacity(), index, index);
		}

		return new (&Values[index]) TValue();
	}

	bool remove(TKey const& key)
	{
		auto index = find_index(key);
		if (index >= 0) {
			this->removeAt(index);
			auto lastIndex = this->Keys.size();
			if (index < (int32_t)lastIndex) {
				Values[index] = std::move(Values[lastIndex]);
				Values[lastIndex].~TValue();
			}

			return true;
		} else {
			return false;
		}
	}

	Iterator begin()
	{
		return Iterator(this, 0);
	}

	ConstIterator begin() const
	{
		return ConstIterator(this, 0);
	}

	Iterator end()
	{
		return Iterator(this, this->Keys.size());
	}

	ConstIterator end() const
	{
		return ConstIterator(this, this->Keys.size());
	}

	Iterator find(TKey const& key)
	{
		auto idx = this->find_index(key);
		return Iterator(this, idx != -1 ? idx : this->Keys.size());
	}

	ConstIterator find(TKey const& key) const
	{
		auto idx = this->find_index(key);
		return ConstIterator(this, idx != -1 ? idx : this->Keys.size());
	}

	TValue const* try_get(TKey const& key) const
	{
		auto index = this->find_index(key);
		if (index == -1) {
			return nullptr;
		} else {
			return &Values[index];
		}
	}

	TValue* try_get(TKey const& key)
	{
		auto index = this->find_index(key);
		if (index == -1) {
			return nullptr;
		} else {
			return &Values[index];
		}
	}

	TValue* get_or_add(TKey const& key)
	{
		auto index = this->find_index(key);
		if (index == -1) {
			return add_key(key);
		} else {
			return &Values[index];
		}
	}

	TValue get_or_default(TKey const& key, TValue const& defaultv = TValue{}) const
	{
		auto index = this->find_index(key);
		if (index == -1) {
			return defaultv;
		} else {
			return Values[index];
		}
	}

	inline int find_index(TKey const& key) const
	{
		return HashSet<TKey>::find_index(key);
	}

private:
	UninitializedStaticArray<TValue> Values;
};

template <class TKey, class TValue>
struct VirtualHashMap : public HashMap<TKey, TValue>
{
	virtual inline void Dummy() {}
};


template <class Allocator = GameMemoryAllocator>
struct BitSet
{
	union {
		uint64_t* Buf{ nullptr };
		uint64_t InlineValue;
	};
	uint32_t Capacity{ 64 };
	uint32_t Size{ 0 };

	inline BitSet() {}

	BitSet(BitSet const& other)
	{
		Reallocate(other.Size);
		Size = other.Size;
		for (uint32_t i = 0; i < other.NumQwords(); i++) {
			GetBuf()[i] = other.GetBuf()[i];
		}
	}

	~BitSet()
	{
		Clear();
		if (Capacity > 64) {
			Allocator::Free(Buf);
		}
	}

	BitSet& operator = (BitSet const& other)
	{
		Clear();
		Reallocate(other.Size);
		Size = other.Size;
		for (uint32_t i = 0; i < other.NumQwords(); i++) {
			GetBuf()[i] = other.GetBuf()[i];
		}
		return *this;
	}

	inline uint32_t NumQwords() const
	{
		return (Size / 64) + ((Size % 64) ? 1 : 0);
	}

	inline uint64_t const* GetBuf() const
	{
		if (Capacity > 64) {
			return Buf;
		} else {
			return &InlineValue;
		}
	}

	inline uint64_t* GetBuf()
	{
		if (Capacity > 64) {
			return Buf;
		} else {
			return &InlineValue;
		}
	}

	inline bool Get(uint32_t index) const
	{
		if (Size < index) {
			return false;
		} else {
			return (GetBuf()[index / 64] & (1ull << (index % 64))) != 0;
		}
	}

	inline bool operator [] (uint32_t index) const
	{
		if (Size < index) {
			return false;
		} else {
			return (GetBuf()[index / 64] & (1ull << (index % 64))) != 0;
		}
	}

	inline void Set(uint32_t index)
	{
		EnsureSize(index + 1);
		GetBuf()[index / 64] |= (1ull << (index % 64));
	}

	inline void Clear(uint32_t index)
	{
		EnsureSize(index + 1);
		GetBuf()[index / 64] &= ~(1ull << (index % 64));
	}

	void EnsureSize(uint32_t size)
	{
		if (Size < size) {
			Reallocate(size);
			Size = size;
		}
	}

	void Reallocate(uint32_t newCapacity)
	{
		newCapacity = ((newCapacity / 64) + ((newCapacity % 64) ? 1 : 0)) * 64;

		if (Capacity == newCapacity) {
			return;
		}

		auto oldCapacity = Capacity;
		auto oldBuf = Buf;
		auto oldInline = InlineValue;

		if (newCapacity > 64) {
			Buf = Allocator::New<uint64_t>(newCapacity);
		} else {
			// Don't touch inline value
		}

		Capacity = newCapacity;

		if (newCapacity < Size) {
			Size = newCapacity;
		}

		if (oldCapacity <= 64 && newCapacity <= 64) {
			// Nothing to do, inline value untouched
		} else if (oldCapacity <= 64 && newCapacity > 64) {
			Buf[0] = oldInline;
		} else if (oldCapacity > 64 && newCapacity <= 64) {
			InlineValue = oldBuf[0];
		} else {
			for (uint32_t i = 0; i < NumQwords(); i++) {
				Buf[i] = oldBuf[i];
			}
		}
				
		if (oldCapacity > 64) {
			Allocator::Free(oldBuf);
		}
	}

	void Clear()
	{
		for (uint32_t i = 0; i < NumQwords(); i++) {
			GetBuf()[i] = 0;
		}

		Size = 0;
	}
};

END_SE()

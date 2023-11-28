#pragma once

#include <cstdint>

BEGIN_SE()

unsigned int GetNearestLowerPrime(unsigned int num);
unsigned int GetNearestMultiHashMapPrime(unsigned int num);

template <class TKey, class TValue>
struct MapNode
{
	MapNode<TKey, TValue>* Next{ nullptr };
	TKey Key;
	TValue Value;

	MapNode() {}

	MapNode(TKey const& key, TValue const& value)
		: Key(key), Value(value)
	{}

	MapNode(TKey const& key, TValue&& value)
		: Key(key), Value(std::move(value))
	{}
};

template <class TKey, class TValue>
struct MapInternals
{
	using KeyType = TKey;
	using ValueType = TValue;

	uint32_t HashSize{ 0 };
	MapNode<TKey, TValue>** HashTable{ nullptr };
	uint32_t ItemCount{ 0 };
};

template <class TKey, class TValue>
struct RefMapInternals
{
	using KeyType = TKey;
	using ValueType = TValue;

	uint32_t ItemCount{ 0 };
	uint32_t HashSize{ 0 };
	MapNode<TKey, TValue>** HashTable{ nullptr };
};

template <class TInternals>
class MapBase : private TInternals
{
public:
	using KeyType = TInternals::KeyType;
	using ValueType = TInternals::ValueType;
	using Node = MapNode<KeyType, ValueType>;

	class Iterator
	{
	public:
		Iterator(MapBase& map)
			: CurrentNode(map.HashTable), NodeListEnd(map.HashTable + map.HashSize), Element(nullptr)
		{
			while (CurrentNode < NodeListEnd && *CurrentNode == nullptr) {
				CurrentNode++;
			}

			if (CurrentNode < NodeListEnd && *CurrentNode) {
				Element = *CurrentNode;
			}
		}
			
		Iterator(MapBase& map, Node** node, Node* element)
			: CurrentNode(node), NodeListEnd(map.HashTable + map.HashSize), Element(element)
		{}

		Iterator operator ++ ()
		{
			Iterator it(*this);

			Element = Element->Next;
			if (Element == nullptr) {
				do {
					CurrentNode++;
				} while (CurrentNode < NodeListEnd && *CurrentNode == nullptr);

				if (CurrentNode < NodeListEnd && *CurrentNode) {
					Element = *CurrentNode;
				}
			}

			return it;
		}

		Iterator& operator ++ (int)
		{
			Element = Element->Next;
			if (Element == nullptr) {
				do {
					CurrentNode++;
				} while (CurrentNode < NodeListEnd && *CurrentNode == nullptr);

				if (CurrentNode < NodeListEnd && *CurrentNode) {
					Element = *CurrentNode;
				}
			}

			return *this;
		}

		bool operator == (Iterator const& it)
		{
			return it.CurrentNode == CurrentNode && it.Element == Element;
		}

		bool operator != (Iterator const& it)
		{
			return it.CurrentNode != CurrentNode || it.Element != Element;
		}

		KeyType & Key () const
		{
			return Element->Key;
		}

		ValueType& Value () const
		{
			return Element->Value;
		}

		Node& operator * () const
		{
			return *Element;
		}

		Node& operator -> () const
		{
			return *Element;
		}

		operator bool() const
		{
			return Element != nullptr;
		}

		bool operator !() const
		{
			return Element == nullptr;
		}

	protected:
		friend class MapBase<TInternals>;

		Node** CurrentNode, ** NodeListEnd;
		Node* Element;
	};

	class ConstIterator
	{
	public:
		ConstIterator(MapBase const& map)
			: CurrentNode(map.HashTable), NodeListEnd(map.HashTable + map.HashSize), Element(nullptr)
		{
			while (CurrentNode < NodeListEnd && *CurrentNode == nullptr) {
				CurrentNode++;
			}

			if (CurrentNode < NodeListEnd && *CurrentNode) {
				Element = *CurrentNode;
			}
		}

		ConstIterator(MapBase const& map, Node* const* node, Node const* element)
			: CurrentNode(node), NodeListEnd(map.HashTable + map.HashSize), Element(element)
		{}

		ConstIterator operator ++ ()
		{
			ConstIterator it(*this);

			Element = Element->Next;
			if (Element == nullptr) {
				do {
					CurrentNode++;
				} while (CurrentNode < NodeListEnd && *CurrentNode == nullptr);

				if (CurrentNode < NodeListEnd && *CurrentNode) {
					Element = *CurrentNode;
				}
			}

			return it;
		}

		ConstIterator& operator ++ (int)
		{
			Element = Element->Next;
			if (Element == nullptr) {
				do {
					CurrentNode++;
				} while (CurrentNode < NodeListEnd && *CurrentNode == nullptr);

				if (CurrentNode < NodeListEnd && *CurrentNode) {
					Element = *CurrentNode;
				}
			}

			return *this;
		}

		bool operator == (ConstIterator const& it)
		{
			return it.CurrentNode == CurrentNode && it.Element == Element;
		}

		bool operator != (ConstIterator const& it)
		{
			return it.CurrentNode != CurrentNode || it.Element != Element;
		}

		KeyType const& Key() const
		{
			return Element->Key;
		}

		ValueType const& Value() const
		{
			return Element->Value;
		}

		Node const& operator * () const
		{
			return *Element;
		}

		Node const& operator -> () const
		{
			return *Element;
		}

		operator bool() const
		{
			return Element != nullptr;
		}

		bool operator !() const
		{
			return Element == nullptr;
		}

	private:
		Node* const * CurrentNode, * const * NodeListEnd;
		Node const* Element;
	};

	MapBase(uint32_t hashSize = 31)
	{
		ResizeHashtable(hashSize);
	}

	MapBase(MapBase<TInternals> const& other)
	{
		ResizeHashtable(other.HashSize);

		for (auto const& pair : other) {
			insert(pair.Key, pair.Value);
		}
	}

	MapBase(MapBase<TInternals> && other)
	{
		this->HashTable = other.HashTable;
		this->HashSize = other.HashSize;
		this->ItemCount = other.ItemCount;

		other.HashTable = nullptr;
		other.HashSize = 0;
		other.ItemCount = 0;
	}

	~MapBase()
	{
		if (this->HashTable) {
			clear();
			GameFree(this->HashTable);
		}
	}

	MapBase<TInternals>& operator =(MapBase<TInternals> const& other)
	{
		ResizeHashtable(other.HashSize);
		for (auto const& pair : other) {
			insert(pair.Key, pair.Value);
		}

		return *this;
	}

	MapBase<TInternals>& operator =(MapBase<TInternals> && other)
	{
		this->HashTable = other.HashTable;
		this->HashSize = other.HashSize;
		this->ItemCount = other.ItemCount;

		other.HashTable = nullptr;
		other.HashSize = 0;
		other.ItemCount = 0;

		return *this;
	}

	void ResizeHashtable(uint32_t hashSize)
	{
		if (this->HashTable) {
			clear();
		}

		if (this->HashSize != hashSize) {
			if (this->HashTable) {
				GameFree(this->HashTable);
			}

			this->HashSize = hashSize;
			this->HashTable = GameAllocArray<Node*>(this->HashSize);
			memset(this->HashTable, 0, sizeof(Node*) * this->HashSize);
		}
	}

	void clear()
	{
		this->ItemCount = 0;
		for (uint32_t i = 0; i < this->HashSize; i++) {
			auto item = this->HashTable[i];
			if (item != nullptr) {
				FreeHashChain(item);
				this->HashTable[i] = nullptr;
			}
		}
	}

	ValueType* insert(KeyType const& key, ValueType const& value) requires std::copyable<ValueType>
	{
		auto* hash = &this->HashTable[Hash(key) % this->HashSize];

		auto item = *hash;
		auto last = item;
		while (item != nullptr) {
			if (key == item->Key) {
				item->Value = value;
				return &item->Value;
			}

			last = item;
			item = item->Next;
		}

		auto node = GameAlloc<Node>(key, value);
		if (last == nullptr) {
			*hash = node;
		} else {
			last->Next = node;
		}

		this->ItemCount++;
		return &node->Value;
	}

	ValueType* insert(KeyType && key, ValueType && value)
	{
		auto* hash = &this->HashTable[Hash(key) % this->HashSize];

		auto item = *hash;
		auto last = item;
		while (item != nullptr) {
			if (key == item->Key) {
				item->Value = std::move(value);
				return &item->Value;
			}

			last = item;
			item = item->Next;
		}

		auto node = GameAlloc<Node>(std::move(key), std::move(value));
		if (last == nullptr) {
			*hash = node;
		} else {
			last->Next = node;
		}

		this->ItemCount++;
		return &node->Value;
	}

	ValueType* insert(std::pair<KeyType, ValueType> const& v) requires std::copyable<ValueType>
	{
		return insert(v.first, v.second);
	}

	ValueType* insert(std::pair<KeyType, ValueType>&& v)
	{
		return insert(std::move(v.first), std::move(v.second));
	}

	ValueType* get_or_insert(KeyType const & key)
	{
		auto* hash = &this->HashTable[Hash(key) % this->HashSize];

		auto item = *hash;
		auto last = item;
		while (item != nullptr) {
			if (key == item->Key) {
				return &item->Value;
			}

			last = item;
			item = item->Next;
		}

		auto node = GameAlloc<Node>(key, ValueType{});
		if (last == nullptr) {
			*hash = node;
		} else {
			last->Next = node;
		}

		this->ItemCount++;
		return &node->Value;
	}

	void erase(Iterator const& it)
	{
		auto elem = *it.CurrentNode;
		if (elem == it.Element) {
			*it.CurrentNode = it.Element->Next;
		} else {
			while (elem->Next != it.Element) {
				elem = elem->Next;
			}

			elem->Next = it.Element->Next;
		}
	}

	Iterator find_by_value(ValueType const& value)
	{
		for (uint32_t bucket = 0; bucket < this->HashSize; bucket++) {
			Node* item = this->HashTable[bucket];
			while (item != nullptr) {
				if (value == item->Value) {
					return Iterator(*this, this->HashTable + bucket, item);
				}

				item = item->Next;
			}
		}

		return end();
	}

	ConstIterator find_by_value(ValueType const& value) const
	{
		for (uint32_t bucket = 0; bucket < this->HashSize; bucket++) {
			Node* item = this->HashTable[bucket];
			while (item != nullptr) {
				if (value == item->Value) {
					return ConstIterator(*this, this->HashTable + bucket, item);
				}

				item = item->Next;
			}
		}

		return end();
	}

	ConstIterator find(KeyType const& key) const
	{
		auto slot = Hash(key) % this->HashSize;
		auto item = this->HashTable[slot];
		while (item != nullptr) {
			if (key == item->Key) {
				return ConstIterator(*this, this->HashTable + slot, item);
			}

			item = item->Next;
		}

		return end();
	}

	Iterator find(KeyType const& key)
	{
		auto slot = Hash(key) % this->HashSize;
		auto item = this->HashTable[slot];
		while (item != nullptr) {
			if (key == item->Key) {
				return Iterator(*this, this->HashTable + slot, item);
			}

			item = item->Next;
		}

		return end();
	}

	ValueType const* try_get_ptr(KeyType const& key) const
	{
		auto it = find(key);
		if (it) {
			return &it.Value();
		} else {
			return nullptr;
		}
	}

	ValueType* try_get_ptr(KeyType const& key)
	{
		auto it = find(key);
		if (it) {
			return &it.Value();
		} else {
			return nullptr;
		}
	}

	ValueType try_get(KeyType const& key, ValueType defaultValue = {}) const
	{
		auto it = find(key);
		if (it) {
			return it.Value();
		} else {
			return defaultValue;
		}
	}

	Iterator begin()
	{
		return Iterator(*this);
	}

	Iterator end()
	{
		return Iterator(*this, this->HashTable + this->HashSize, nullptr);
	}

	ConstIterator begin() const
	{
		return ConstIterator(*this);
	}

	ConstIterator end() const
	{
		return ConstIterator(*this, this->HashTable + this->HashSize, nullptr);
	}

	inline uint32_t size() const
	{
		return this->ItemCount;
	}

private:
	void FreeHashChain(Node* node)
	{
		do {
			auto next = node->Next;
			GameDelete(node);
			node = next;
		} while (node != nullptr);
	}
};

template <class TKey, class TValue>
using Map = MapBase<MapInternals<TKey, TValue>>;

template <class TKey, class TValue>
using RefMap = MapBase<RefMapInternals<TKey, TValue>>;

// Special hashing needed for FixedStrings in the new hash table

template <class T>
inline uint64_t MultiHashMapHash(T const& v)
{
	return Hash(v);
}

template <>
inline uint64_t MultiHashMapHash<FixedString>(FixedString const& v)
{
	return v.GetHash();
}

template <class T>
struct MultiHashSet
{
	using value_type = T;
	using reference = T&;
	using const_reference = T const&;
	using iterator = ContiguousIterator<T>;
	using const_iterator = ContiguousConstIterator<T>;
	using difference_type = int32_t;
	using size_type = uint32_t;

	StaticArray<int32_t> HashKeys;
	Array<int32_t> NextIds;
	Array<T> Keys;

	MultiHashSet()
	{}

	MultiHashSet(MultiHashSet const& other)
		: HashKeys(other.HashKeys), NextIds(other.NextIds), Keys(other.Keys)
	{}

	MultiHashSet(MultiHashSet&& other)
		: HashKeys(std::move(other.HashKeys)), NextIds(std::move(other.NextIds)), Keys(std::move(other.Keys))
	{}

	~MultiHashSet()
	{}

	MultiHashSet& operator =(MultiHashSet const& other)
	{
		HashKeys = other.HashKeys;
		NextIds = other.NextIds;
		Keys = other.Keys;
		return *this;
	}

	MultiHashSet& operator =(MultiHashSet&& other)
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

	void clear()
	{
		Keys.clear();
		std::fill(HashKeys.begin(), HashKeys.end(), -1);
		std::fill(NextIds.begin(), NextIds.end(), -1);
	}

	int FindIndex(T const& key) const
	{
		if (HashKeys.Size() == 0) return -1;

		auto keyIndex = HashKeys[MultiHashMapHash(key) % HashKeys.Size()];
		while (keyIndex >= 0) {
			if (Keys[keyIndex] == key) return keyIndex;
			keyIndex = NextIds[keyIndex];
		}

		return -1;
	}

	bool Contains(T const& key) const
	{
		return FindIndex(key) != -1;
	}

	void Clear()
	{
		std::fill(HashKeys.begin(), HashKeys.end(), -1);
		NextIds.clear();
		Keys.clear();
	}

	int Add(T const& key)
	{
		auto index = FindIndex(key);
		if (index != -1) {
			return index;
		}

		int keyIdx = (int)Keys.Size();
		Keys.Add(key);

		if (NextIds.size() < Keys.size()) {
			NextIds.resize(Keys.size());
		}

		NextIds[keyIdx] = -1;

		if (HashKeys.Size() >= Keys.Size() * 2) {
			InsertToHashMap(key, keyIdx);
		} else {
			ResizeHashMap(2 * (unsigned)Keys.Size());
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
		auto idx = FindIndex(key);
		return idx != -1 ? (Keys.begin() + idx) : Keys.end();
	}

	ContiguousIterator<T> find(T const& key)
	{
		auto idx = FindIndex(key);
		return idx != -1 ? (Keys.begin() + idx) : Keys.end();
	}

	bool remove(T const& key)
	{
		return removeKeyInternal(key) >= 0;
	}

protected:
	int removeKeyInternal(T const& key)
	{
		if (HashKeys.Size() == 0) return -1;

		auto hash = (uint32_t)(MultiHashMapHash(key) % HashKeys.Size());
		auto keyIndex = HashKeys[hash];
		auto prevKeyIndex = keyIndex;
		while (keyIndex >= 0) {
			if (Keys[keyIndex] == key) {
				if (prevKeyIndex == keyIndex) {
					// Remove first element in the hash table
					HashKeys[hash] = NextIds[keyIndex];
				} else {
					// Remove intermediate element in the chain
					NextIds[prevKeyIndex] = NextIds[keyIndex];
				}

				NextIds[keyIndex] = -1;
				Keys[keyIndex] = T{};
				if (keyIndex != size() - 1) {
					migrateKey(size() - 1, keyIndex);
				}

				Keys.pop_last();
				return keyIndex;
			}

			prevKeyIndex = keyIndex;
			keyIndex = NextIds[keyIndex];
		}

		return -1;
	}

	void migrateKey(int32_t from, int32_t to)
	{
		// Backrefs in NextIds are inconsistent, so we need to rehash the key we're moving
		auto slot = (uint32_t)(MultiHashMapHash(Keys[from]) % HashKeys.Size());

		Keys[to] = std::move(Keys[from]);

		int32_t prev = HashKeys[slot];
		if (prev == from) {
			HashKeys[slot] = to;
		} else {
			while (NextIds[prev] != from) {
				prev = NextIds[prev];
			}

			assert(NextIds[prev] == from);
			NextIds[prev] = to;
		}
	}

private:
	void InsertToHashMap(T const& key, int keyIdx)
	{
		auto bucket = (uint32_t)(MultiHashMapHash(key) % HashKeys.Size());
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
		for (unsigned k = 0; k < Keys.Size(); k++) {
			InsertToHashMap(Keys[k], k);
		}
	}
};

template <class T>
struct VirtualMultiHashSet : public MultiHashSet<T>
{
	virtual inline void Dummy() {}
};

template <class TKey, class TValue>
struct MultiHashMap : public MultiHashSet<TKey>
{
	UninitializedStaticArray<TValue> Values;

	class ConstIterator
	{
	public:
		ConstIterator(MultiHashMap const* map)
			: Map(map), Index(0)
		{}
			
		ConstIterator(MultiHashMap const* map, int index)
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
		MultiHashMap const* Map;
		int32_t Index;
	};

	class Iterator
	{
	public:
		Iterator(MultiHashMap * map)
			: Map(map), Index(0)
		{}
			
		Iterator(MultiHashMap * map, int index)
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

		TKey & Key() const
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
		MultiHashMap* Map;
		int32_t Index;
	};

	MultiHashMap()
	{}

	MultiHashMap(MultiHashMap const& other)
		: MultiHashSet<TKey>(other)
	{
		Values.copy_from(other.Values, 0, other.Keys.size());
	}

	MultiHashMap(MultiHashMap&& other)
		: MultiHashSet<TKey>(std::move(other)),
		Values(std::move(other.Values))
	{}

	~MultiHashMap()
	{
		Values.clear(this->Keys.size());
	}

	MultiHashMap& operator =(MultiHashMap const& other)
	{
		auto oldSize = this->Keys.size();
		MultiHashSet<TKey>::operator =(other);
		Values.copy_from(other.Values, oldSize, other.Keys.size());

		return *this;
	}

	MultiHashMap& operator =(MultiHashMap&& other)
	{
		MultiHashSet<TKey>::operator =(std::move(other));
		Values = std::move(other.Values);

		return *this;
	}

	void clear()
	{
		Values.clear(this->Keys.size());
		MultiHashSet<TKey>::clear();
	}

	std::optional<TValue const*> Find(TKey const& key) const
	{
		auto index = this->FindIndex(key);
		if (index == -1) {
			return {};
		} else {
			return &Values[index];
		}
	}

	std::optional<TValue*> Find(TKey const& key)
	{
		auto index = this->FindIndex(key);
		if (index == -1) {
			return {};
		} else {
			return &Values[index];
		}
	}

	TValue* Set(TKey const& key, TValue&& value)
	{
		auto index = this->Add(key);
		if (Values.size() <= (uint32_t)index) {
			Values.resize(Values.grow_size(), index, index);
		}

		return new (&Values[index]) TValue(std::move(value));
	}

	TValue* Set(TKey const& key, TValue const& value)
	{
		auto index = this->Add(key);
		if (Values.size() <= (uint32_t)index) {
			Values.resize(Values.grow_size(), index, index);
		}

		return new (&Values[index]) TValue(value);
	}

	bool remove(TKey const& key)
	{
		auto index = this->removeKeyInternal(key);
		if (index >= 0) {
			auto lastIndex = this->Keys.size();
			if (index < (int32_t)this->Keys.size()) {
				Values[index] = std::move(Values[this->Keys.size()]);
				Values[this->Keys.size()].~TValue();
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
		return Iterator(this, this->Keys.Size());
	}

	ConstIterator end() const
	{
		return ConstIterator(this, this->Keys.Size());
	}

	Iterator find(TKey const& key)
	{
		auto idx = this->FindIndex(key);
		return Iterator(this, idx != -1 ? idx : this->Keys.size());
	}

	ConstIterator find(TKey const& key) const
	{
		auto idx = this->FindIndex(key);
		return ConstIterator(this, idx != -1 ? idx : this->Keys.size());
	}
};

template <class TKey, class TValue>
struct VirtualMultiHashMap : public MultiHashMap<TKey, TValue>
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

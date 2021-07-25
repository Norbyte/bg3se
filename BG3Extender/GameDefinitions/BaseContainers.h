#pragma once

#include <cstdint>

#include <GameDefinitions/BaseUtilities.h>
#include <GameDefinitions/BaseMemory.h>

namespace bg3se
{
	unsigned int GetNearestLowerPrime(unsigned int num);
	unsigned int GetNearestMultiHashMapPrime(unsigned int num);

	template <class T>
	struct ContiguousIterator
	{
		T* Ptr;

		ContiguousIterator(T* p) : Ptr(p) {}

		ContiguousIterator operator ++ ()
		{
			ContiguousIterator<T> it(Ptr);
			Ptr++;
			return it;
		}

		ContiguousIterator& operator ++ (int)
		{
			Ptr++;
			return *this;
		}

		bool operator == (ContiguousIterator const& it)
		{
			return it.Ptr == Ptr;
		}

		bool operator != (ContiguousIterator const& it)
		{
			return it.Ptr != Ptr;
		}

		T& operator * ()
		{
			return *Ptr;
		}

		T* operator -> ()
		{
			return Ptr;
		}
	};


	template <class T>
	struct ContiguousConstIterator
	{
		T const* Ptr;

		ContiguousConstIterator(T const* p) : Ptr(p) {}

		ContiguousConstIterator operator ++ ()
		{
			ContiguousConstIterator<T> it(Ptr);
			Ptr++;
			return it;
		}

		ContiguousConstIterator& operator ++ (int)
		{
			Ptr++;
			return *this;
		}

		bool operator == (ContiguousConstIterator const& it)
		{
			return it.Ptr == Ptr;
		}

		bool operator != (ContiguousConstIterator const& it)
		{
			return it.Ptr != Ptr;
		}

		T const& operator * ()
		{
			return *Ptr;
		}

		T const* operator -> ()
		{
			return Ptr;
		}
	};


	template <class TKey, class TValue>
	class Map : public Noncopyable<Map<TKey, TValue>>
	{
	public:
		struct Node
		{
			Node* Next{ nullptr };
			TKey Key;
			TValue Value;
		};

		class Iterator
		{
		public:
			Iterator(Map& map) 
				: Node(map.HashTable), NodeListEnd(map.HashTable + map.HashSize), Element(nullptr)
			{
				while (Node < NodeListEnd && *Node == nullptr) {
					Node++;
				}

				if (Node < NodeListEnd && *Node) {
					Element = *Node;
				}
			}
			
			Iterator(Map& map, Node** node, Node* element)
				: Node(node), NodeListEnd(map.HashTable + map.HashSize), Element(element)
			{}

			Iterator operator ++ ()
			{
				Iterator it(*this);

				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return it;
			}

			Iterator& operator ++ (int)
			{
				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return *this;
			}

			bool operator == (Iterator const& it)
			{
				return it.Node == Node && it.Element == Element;
			}

			bool operator != (Iterator const& it)
			{
				return it.Node != Node || it.Element != Element;
			}

			TKey & Key () const
			{
				return Element->Key;
			}

			TValue & Value () const
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

		private:
			Node** Node, ** NodeListEnd;
			Map<TKey, TValue>::Node* Element;
		};

		class ConstIterator
		{
		public:
			ConstIterator(Map const& map)
				: Node(map.HashTable), NodeListEnd(map.HashTable + map.HashSize), Element(nullptr)
			{
				while (Node < NodeListEnd && *Node == nullptr) {
					Node++;
				}

				if (Node < NodeListEnd && *Node) {
					Element = *Node;
				}
			}

			ConstIterator(Map const& map, Node* const* node, Node const* element)
				: Node(node), NodeListEnd(map.HashTable + map.HashSize), Element(element)
			{}

			ConstIterator operator ++ ()
			{
				ConstIterator it(*this);

				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return it;
			}

			ConstIterator& operator ++ (int)
			{
				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return *this;
			}

			bool operator == (ConstIterator const& it)
			{
				return it.Node == Node && it.Element == Element;
			}

			bool operator != (ConstIterator const& it)
			{
				return it.Node != Node || it.Element != Element;
			}

			TKey const& Key() const
			{
				return Element->Key;
			}

			TValue const& Value() const
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

		private:
			Node* const * Node, * const * NodeListEnd;
			Map<TKey, TValue>::Node const* Element;
		};

		Map() {}

		Map(uint32_t hashSize)
		{
			Init(hashSize);
		}

		~Map()
		{
			Clear();
		}

		void Init(uint32_t hashSize)
		{
			HashSize = hashSize;
			HashTable = GameAllocArray<Node*>(hashSize);
			ItemCount = 0;
			memset(HashTable, 0, sizeof(Node*) * hashSize);
		}

		void Clear()
		{
			ItemCount = 0;
			for (uint32_t i = 0; i < HashSize; i++) {
				auto item = HashTable[i];
				if (item != nullptr) {
					FreeHashChain(item);
					HashTable[i] = nullptr;
				}
			}
		}

		void FreeHashChain(Node* node)
		{
			do {
				auto next = node->Next;
				GameDelete(node);
				node = next;
			} while (node != nullptr);
		}

		TValue* Insert(TKey const& key, TValue const& value)
		{
			auto nodeValue = Insert(key);
			*nodeValue = value;
			return nodeValue;
		}

		TValue* Insert(TKey const& key)
		{
			auto item = HashTable[Hash(key) % HashSize];
			auto last = item;
			while (item != nullptr) {
				if (key == item->Key) {
					return &item->Value;
				}

				last = item;
				item = item->Next;
			}

			auto node = GameAlloc<Node>();
			node->Next = nullptr;
			node->Key = key;

			if (last == nullptr) {
				HashTable[Hash(key) % HashSize] = node;
			}
			else {
				last->Next = node;
			}

			ItemCount++;
			return &node->Value;
		}

		TValue* Find(TKey const& key) const
		{
			auto item = HashTable[Hash(key) % HashSize];
			while (item != nullptr) {
				if (key == item->Key) {
					return &item->Value;
				}

				item = item->Next;
			}

			return nullptr;
		}

		TKey* FindByValue(TValue const& value) const
		{
			for (uint32_t bucket = 0; bucket < HashSize; bucket++) {
				Node* item = HashTable[bucket];
				while (item != nullptr) {
					if (value == item->Value) {
						return &item->Key;
					}

					item = item->Next;
				}
			}

			return nullptr;
		}

		template <class Visitor>
		void Iterate(Visitor visitor)
		{
			for (uint32_t bucket = 0; bucket < HashSize; bucket++) {
				Node* item = HashTable[bucket];
				while (item != nullptr) {
					visitor(item->Key, item->Value);
					item = item->Next;
				}
			}
		}

		template <class Visitor>
		void Iterate(Visitor visitor) const
		{
			for (uint32_t bucket = 0; bucket < HashSize; bucket++) {
				Node* item = HashTable[bucket];
				while (item != nullptr) {
					visitor(item->Key, item->Value);
					item = item->Next;
				}
			}
		}

		Iterator begin()
		{
			return Iterator(*this);
		}

		Iterator end()
		{
			return Iterator(*this, HashTable + HashSize, nullptr);
		}

		ConstIterator begin() const
		{
			return ConstIterator(*this);
		}

		ConstIterator end() const
		{
			return ConstIterator(*this, HashTable + HashSize, nullptr);
		}

		inline uint32_t Count() const
		{
			return ItemCount;
		}

	private:
		uint32_t HashSize{ 0 };
		Node** HashTable{ nullptr };
		uint32_t ItemCount{ 0 };
	};

	template <class TKey, class TValue>
	class RefMap : public Noncopyable<RefMap<TKey, TValue>>
	{
	public:
		struct Node
		{
			Node* Next{ nullptr };
			TKey Key;
			TValue Value;
		};

		class Iterator
		{
		public:
			Iterator(RefMap& map) 
				: Node(map.HashTable), NodeListEnd(map.HashTable + map.HashSize), Element(nullptr)
			{
				while (Node < NodeListEnd && *Node == nullptr) {
					Node++;
				}

				if (Node < NodeListEnd && *Node) {
					Element = *Node;
				}
			}
			
			Iterator(RefMap& map, Node** node, Node* element)
				: Node(node), NodeListEnd(map.HashTable + map.HashSize), Element(element)
			{}

			Iterator operator ++ ()
			{
				Iterator it(*this);

				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return it;
			}

			Iterator& operator ++ (int)
			{
				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return *this;
			}

			bool operator == (Iterator const& it)
			{
				return it.Node == Node && it.Element == Element;
			}

			bool operator != (Iterator const& it)
			{
				return it.Node != Node || it.Element != Element;
			}

			TKey & Key () const
			{
				return Element->Key;
			}

			TValue & Value () const
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

		private:
			Node** Node, ** NodeListEnd;
			RefMap<TKey, TValue>::Node* Element;
		};

		class ConstIterator
		{
		public:
			ConstIterator(RefMap const& map)
				: Node(map.HashTable), NodeListEnd(map.HashTable + map.HashSize), Element(nullptr)
			{
				while (Node < NodeListEnd && *Node == nullptr) {
					Node++;
				}

				if (Node < NodeListEnd && *Node) {
					Element = *Node;
				}
			}

			ConstIterator(RefMap const& map, Node* const* node, Node const* element)
				: Node(node), NodeListEnd(map.HashTable + map.HashSize), Element(element)
			{}

			ConstIterator operator ++ ()
			{
				ConstIterator it(*this);

				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return it;
			}

			ConstIterator& operator ++ (int)
			{
				Element = Element->Next;
				if (Element == nullptr) {
					do {
						Node++;
					} while (Node < NodeListEnd && *Node == nullptr);

					if (Node < NodeListEnd && *Node) {
						Element = *Node;
					}
				}

				return *this;
			}

			bool operator == (ConstIterator const& it)
			{
				return it.Node == Node && it.Element == Element;
			}

			bool operator != (ConstIterator const& it)
			{
				return it.Node != Node || it.Element != Element;
			}

			TKey const& Key() const
			{
				return Element->Key;
			}

			TValue const& Value() const
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

		private:
			Node* const * Node, * const * NodeListEnd;
			RefMap<TKey, TValue>::Node const* Element;
		};

		RefMap(uint32_t hashSize = 31)
			: ItemCount(0), HashSize(hashSize)
		{
			HashTable = GameAllocArray<Node*>(hashSize);
			memset(HashTable, 0, sizeof(Node*) * hashSize);
		}

		~RefMap()
		{
			if (HashTable != nullptr) {
				GameFree(HashTable);
			}
		}

		Iterator begin()
		{
			return Iterator(*this);
		}

		Iterator end()
		{
			return Iterator(*this, HashTable + HashSize, nullptr);
		}

		ConstIterator begin() const
		{
			return ConstIterator(*this);
		}

		ConstIterator end() const
		{
			return ConstIterator(*this, HashTable + HashSize, nullptr);
		}

		inline uint32_t Count() const
		{
			return ItemCount;
		}

		void Clear()
		{
			ItemCount = 0;
			for (uint32_t i = 0; i < HashSize; i++) {
				auto item = HashTable[i];
				if (item != nullptr) {
					FreeHashChain(item);
					HashTable[i] = nullptr;
				}
			}
		}

		void FreeHashChain(Node* node)
		{
			do {
				auto next = node->Next;
				GameDelete(node);
				node = next;
			} while (node != nullptr);
		}

		TValue* Find(TKey const& key) const
		{
			auto item = HashTable[Hash(key) % HashSize];
			while (item != nullptr) {
				if (key == item->Key) {
					return &item->Value;
				}

				item = item->Next;
			}

			return nullptr;
		}

		ConstIterator FindIterator(TKey const& key) const
		{
			auto slot = Hash(key) % HashSize;
			auto item = HashTable[slot];
			while (item != nullptr) {
				if (key == item->Key) {
					return ConstIterator(*this, HashTable + slot, item);
				}

				item = item->Next;
			}

			return end();
		}

		Iterator FindIterator(TKey const& key)
		{
			auto slot = Hash(key) % HashSize;
			auto item = HashTable[slot];
			while (item != nullptr) {
				if (key == item->Key) {
					return Iterator(*this, HashTable + slot, item);
				}

				item = item->Next;
			}

			return end();
		}

		TValue* Insert(TKey const& key, TValue const& value)
		{
			auto nodeValue = Insert(key);
			*nodeValue = value;
			return nodeValue;
		}

		TValue* Insert(TKey const& key)
		{
			auto item = HashTable[Hash(key) % HashSize];
			auto last = item;
			while (item != nullptr) {
				if (key == item->Key) {
					return &item->Value;
				}

				last = item;
				item = item->Next;
			}

			auto node = GameAlloc<Node>();
			node->Next = nullptr;
			node->Key = key;

			if (last == nullptr) {
				HashTable[Hash(key) % HashSize] = node;
			}
			else {
				last->Next = node;
			}

			ItemCount++;
			return &node->Value;
		}

		template <class Visitor>
		void Iterate(Visitor visitor)
		{
			for (uint32_t bucket = 0; bucket < HashSize; bucket++) {
				Node* item = HashTable[bucket];
				while (item != nullptr) {
					visitor(item->Key, item->Value);
					item = item->Next;
				}
			}
		}

	private:
		uint32_t ItemCount{ 0 };
		uint32_t HashSize{ 0 };
		Node** HashTable{ nullptr };
	};


	template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false>
	struct CompactSet
	{
		T* Buf{ nullptr };
		uint32_t Capacity{ 0 };
		uint32_t Size{ 0 };

		inline CompactSet() {}

		CompactSet(CompactSet const& other)
		{
			Reallocate(other.Size);
			Size = other.Size;
			for (uint32_t i = 0; i < other.Size; i++) {
				new (Buf + i) T(other.Buf[i]);
			}
		}

		~CompactSet()
		{
			if (Buf) {
				Clear();
				FreeBuffer(Buf);
			}
		}

		CompactSet& operator = (CompactSet const& other)
		{
			Clear();
			Reallocate(other.Size);
			Size = other.Size;
			for (uint32_t i = 0; i < other.Size; i++) {
				new (Buf + i) T(other.Buf[i]);
			}
			return *this;
		}

		inline T const& operator [] (uint32_t index) const
		{
			return Buf[index];
		}

		inline T& operator [] (uint32_t index)
		{
			return Buf[index];
		}

		void FreeBuffer(void* buf)
		{
			if (StoreSize) {
				if (buf != nullptr) {
					Allocator::Free((void*)((std::ptrdiff_t)buf - 8));
				}
			} else {
				if (buf != nullptr) {
					Allocator::Free(buf);
				}
			}
		}

		void RawReallocate(uint32_t newCapacity)
		{
			if (newCapacity > 0) {
				if (StoreSize) {
					auto newBuf = Allocator::Alloc(newCapacity * sizeof(T) + 8);
					*(uint64_t*)newBuf = newCapacity;

					Buf = (T*)((std::ptrdiff_t)newBuf + 8);
				}
				else {
					Buf = Allocator::New<T>(newCapacity);
				}
			} else {
				Buf = nullptr;
			}

			Capacity = newCapacity;
		}

		void Reallocate(uint32_t newCapacity)
		{
			auto oldBuf = Buf;
			RawReallocate(newCapacity);

			for (uint32_t i = 0; i < std::min(Size, newCapacity); i++) {
				new (Buf + i) T(oldBuf[i]);
			}

			for (uint32_t i = 0; i < Size; i++) {
				oldBuf[i].~T();
			}

			FreeBuffer(oldBuf);
		}

		void Remove(uint32_t index)
		{
			if (index >= Size) {
				ERR("Tried to remove out-of-bounds index %d!", index);
				return;
			}

			for (auto i = index; i < Size - 1; i++) {
				Buf[i] = Buf[i + 1];
			}

			Buf[Size - 1].~T();
			Size--;
		}

		void Clear()
		{
			for (uint32_t i = 0; i < Size; i++) {
				Buf[i].~T();
			}

			Size = 0;
		}

		ContiguousIterator<T> begin()
		{
			return ContiguousIterator<T>(Buf);
		}

		ContiguousConstIterator<T> begin() const
		{
			return ContiguousConstIterator<T>(Buf);
		}

		ContiguousIterator<T> end()
		{
			return ContiguousIterator<T>(Buf + Size);
		}

		ContiguousConstIterator<T> end() const
		{
			return ContiguousConstIterator<T>(Buf + Size);
		}
	};

	template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false>
	struct Set : public CompactSet<T, Allocator, StoreSize>
	{
		uint64_t CapacityIncrementSize{ 0 };

		uint32_t CapacityIncrement() const
		{
			if (CapacityIncrementSize != 0) {
				return Capacity + (uint32_t)CapacityIncrementSize;
			}
			else if (Capacity > 0) {
				return 2 * Capacity;
			}
			else {
				return 1;
			}
		}

		void Add(T const& value)
		{
			if (Capacity <= Size) {
				Reallocate(CapacityIncrement());
			}

			new (&Buf[Size++]) T(value);
		}

		void InsertAt(uint32_t index, T const& value)
		{
			if (Capacity <= Size) {
				Reallocate(CapacityIncrement());
			}

			for (auto i = Size; i > index; i--) {
				Buf[i] = Buf[i - 1];
			}

			Buf[index] = value;
			Size++;
		}
	};

	template <class T, class Allocator = GameMemoryAllocator>
	struct PrimitiveSmallSet : public CompactSet<T, Allocator, false>
	{
		virtual ~PrimitiveSmallSet() {}

		uint32_t CapacityIncrement() const
		{
			if (Capacity > 0) {
				return 2 * Capacity;
			}
			else {
				return 1;
			}
		}

		void Add(T const& value)
		{
			if (Capacity <= Size) {
				Reallocate(CapacityIncrement());
			}

			new (&Buf[Size++]) T(value);
		}
	};

	template <class T, class Allocator = GameMemoryAllocator, bool StoreSize = false>
	struct ObjectSet : public Set<T, Allocator, StoreSize>
	{
	};

	template <class T, class Allocator = GameMemoryAllocator>
	struct PrimitiveSet : public ObjectSet<T, Allocator, false>
	{
	};

	template <unsigned TDWords>
	struct BitArray
	{
		uint32_t Bits[TDWords];

		inline bool Set(uint32_t index)
		{
			if (index <= 0 || index > (TDWords * 32)) {
				return false;
			}

			Bits[(index - 1) >> 5] |= (1 << ((index - 1) & 0x1f));
			return true;
		}

		inline bool Clear(uint32_t index)
		{
			if (index <= 0 || index > (TDWords * 32)) {
				return false;
			}

			Bits[(index - 1) >> 5] &= ~(1 << ((index - 1) & 0x1f));
			return true;
		}

		inline bool IsSet(uint32_t index) const
		{
			if (index <= 0 || index > (TDWords * 32)) {
				return false;
			}

			return (Bits[(index - 1) >> 5] & (1 << ((index - 1) & 0x1f))) != 0;
		}
	};

	template <class T>
	struct ArrayBase
	{
		T* buf_{ nullptr };
		unsigned int capacity_{ 0 };
	};

	template <class T>
	class Array : private ArrayBase<T>
	{
	public:
		inline Array() {}

		Array(Array const& a)
		{
			CopyFrom(a);
		}

		~Array()
		{
			if (buf_) {
				Clear();
				GameFree(buf_);
			}
		}

		Array& operator =(Array const& a)
		{
			CopyFrom(a);
			return *this;
		}

		void CopyFrom(Array const& a)
		{
			Clear();

			if (a.size_ > 0) {
				Reallocate(a.size_);
				size_ = a.size_;
				for (uint32_t i = 0; i < size_; i++) {
					new (buf_ + i) T(a[i]);
				}
			}
		}

		inline unsigned int Size() const
		{
			return size_;
		}

		inline T const& operator [] (uint32_t index) const
		{
			return buf_[index];
		}

		inline T& operator [] (uint32_t index)
		{
			return buf_[index];
		}

		uint32_t CapacityIncrement() const
		{
			if (capacity_ > 0) {
				return 2 * capacity_;
			} else {
				return 1;
			}
		}

		void Clear()
		{
			for (uint32_t i = 0; i < size_; i++) {
				buf_[i].~T();
			}

			size_ = 0;
		}

		void Reallocate(uint32_t newCapacity)
		{
			auto newBuf = GameMemoryAllocator::NewRaw<T>(newCapacity);
			for (uint32_t i = 0; i < std::min(size_, newCapacity); i++) {
				new (newBuf + i) T(buf_[i]);
			}

			if (buf_ != nullptr) {
				for (uint32_t i = 0; i < size_; i++) {
					buf_[i].~T();
				}

				GameFree(buf_);
			}

			buf_ = newBuf;
			capacity_ = newCapacity;
		}

		void Add(T const& value)
		{
			if (capacity_ <= size_) {
				Reallocate(CapacityIncrement());
			}

			new (&buf_[size_++]) T(value);
		}

		void Remove(uint32_t index)
		{
			if (index >= size_) {
				ERR("Tried to remove out-of-bounds index %d!", index);
				return;
			}

			for (auto i = index; i < size_ - 1; i++) {
				buf_[i] = buf_[i + 1];
			}

			buf_[size_ - 1].~T();
			size_--;
		}

		ContiguousIterator<T> begin()
		{
			return ContiguousIterator<T>(buf_);
		}

		ContiguousConstIterator<T> begin() const
		{
			return ContiguousConstIterator<T>(buf_);
		}

		ContiguousIterator<T> end()
		{
			return ContiguousIterator<T>(buf_ + size_);
		}

		ContiguousConstIterator<T> end() const
		{
			return ContiguousConstIterator<T>(buf_ + size_);
		}

	private:
		unsigned int size_{ 0 };
	};

	template <class T>
	class VirtualArray : public Array<T>
	{
	public:
		inline virtual ~VirtualArray() {};
	};

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
		int32_t* HashKeys{ nullptr };
		uint32_t NumHashKeys{ 0 };
		Array<int32_t> NextIds;
		Array<T> Keys;

		MultiHashSet()
		{}

		MultiHashSet(MultiHashSet const& other)
			: HashKeys(nullptr), NumHashKeys(other.NumHashKeys), NextIds(other.NextIds), Keys(other.Keys)
		{
			if (other.HashKeys) {
				HashKeys = GameAllocArray<int>(NumHashKeys);
				std::copy(other.HashKeys, other.HashKeys + other.NumHashKeys, HashKeys);
			}
		}

		~MultiHashSet()
		{
			if (HashKeys) {
				GameFree(HashKeys);
			}
		}

		MultiHashSet& operator =(MultiHashSet const& other)
		{
			if (HashKeys) {
				GameFree(HashKeys);
				HashKeys = nullptr;
			}

			NextIds = other.NextIds;
			Keys = other.Keys;

			NumHashKeys = other.NumHashKeys;
			if (other.HashKeys) {
				HashKeys = GameAllocArray<int>(NumHashKeys);
				std::copy(other.HashKeys, other.HashKeys + other.NumHashKeys, HashKeys);
			}

			return *this;
		}

		int FindIndex(T const& key) const
		{
			if (NumHashKeys == 0) return -1;

			auto keyIndex = HashKeys[MultiHashMapHash(key) % NumHashKeys];
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
			std::fill(HashKeys, HashKeys + NumHashKeys, -1);
			NextIds.Clear();
			Keys.Clear();
		}

		int Add(T const& key)
		{
			auto index = FindIndex(key);
			if (index != -1) {
				return index;
			}

			int keyIdx = (int)Keys.Size();
			Keys.Add(key);
			NextIds.Add(-1);

			if (NumHashKeys >= Keys.Size() * 2) {
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

	private:
		void InsertToHashMap(T const& key, int keyIdx)
		{
			auto bucket = MultiHashMapHash(key) % NumHashKeys;
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
			if (HashKeys) {
				GameFree(HashKeys);
			}

			HashKeys = GameAllocArray<int32_t>(numBuckets, -1);
			NumHashKeys = numBuckets;
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
		TValue* Values{ nullptr };
		int32_t NumValues{ 0 };

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

		private:
			MultiHashMap* Map;
			int32_t Index;
		};

		MultiHashMap()
		{}

		MultiHashMap(MultiHashMap const& other)
			: MultiHashSet(other)
		{
			NumValues = other.NumValues;
			if (other.Values) {
				Values = GameAllocArray<TValue>(NumValues);
				for (auto i = 0; i < NumValues; i++) {
					new (Values + i) TValue(other.Values[i]);
				}
			}
		}

		~MultiHashMap()
		{
			FreeValues();
		}

		MultiHashMap& operator =(MultiHashMap const& other)
		{
			FreeValues();

			NumValues = other.NumValues;
			if (other.Values) {
				Values = GameAllocArray<TValue>(NumValues);
				for (auto i = 0; i < NumValues; i++) {
					new (Values + i) TValue(other.Values[i]);
				}
			}

			return *this;
		}

		std::optional<TValue const*> Find(TKey const& key) const
		{
			auto index = FindIndex(key);
			if (index == -1) {
				return {};
			} else {
				return Values + index;
			}
		}

		std::optional<TValue*> Find(TKey const& key)
		{
			auto index = FindIndex(key);
			if (index == -1) {
				return {};
			} else {
				return Values + index;
			}
		}

		TValue* Set(TKey const& key, TValue const& value)
		{
			auto index = FindIndex(key);
			if (index == -1) {
				index = Add(key);
				if (NumValues <= index) {
					ResizeValues(index + 1);
					NumValues = index + 1;
				}
			}

			Values[index] = value;
			return &Values[index];
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
			return Iterator(this, NumValues);
		}

		ConstIterator end() const
		{
			return ConstIterator(this, NumValues);
		}

	private:
		void ResizeValues(int32_t newSize)
		{
			auto newBuf = GameMemoryAllocator::NewRaw<TValue>(newSize);
			for (int32_t i = 0; i < std::min(NumValues, newSize); i++) {
				new (newBuf + i) TValue(Values[i]);
			}
			
			for (int32_t i = NumValues; i < newSize; i++) {
				new (newBuf + i) TValue();
			}

			if (Values != nullptr) {
				for (int32_t i = 0; i < NumValues; i++) {
					Values[i].~TValue();
				}

				GameFree(Values);
			}

			Values = newBuf;
			NumValues = newSize;
		}

		void FreeValues()
		{
			if (Values) {
				for (int32_t i = 0; i < NumValues; i++) {
					Values[i].~TValue();
				}

				GameFree(Values);
			}
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

		inline void Set(uint32_t index) const
		{
			EnsureSize(index + 1);
			GetBuf()[index / 64] |= (1ull << (index % 64));
		}

		inline void Clear(uint32_t index) const
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
}
